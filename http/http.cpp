#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <uv.h>
#include <rackcpp.h>

HttpServer::HttpServer(Middleware* middleware, const std::string _bindAddr, int port)
    :middleware(
    middleware), bindAddr(_bindAddr), port(port)
{
    uv_tcp_init(uv_default_loop(), &server);
    uv_ip4_addr(_bindAddr.c_str(), port, &addr);
    uv_tcp_bind(&server, (const sockaddr*) &addr, 0);
    server.data = this;
}

HttpServer::~HttpServer()
{
    delete middleware;
}

static void __closeCallback(uv_handle_t* handle)
{
    delete (Request*) handle->data;
    delete (uv_tcp_t*) handle;
}

static void __allocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

static void __readCallback(uv_stream_t* client, ssize_t nread, const uv_buf_t* buf)
{
    Request* req = (Request*) client->data;
    if (nread > 0)
    {
        req->push_buf(buf, nread);
    }
    if (nread < 0)
    {
        if (nread != UV_EOF)
            Logger::getInstance().error(std::string("Read error: ") + uv_strerror((int) nread) + "\n");
        uv_close((uv_handle_t*) client, __closeCallback);
    }
    delete[] buf->base;
}

struct write_response_req
{
    uv_write_t wt;
    HttpServer* server;
};

static void __writeCallback(uv_write_t* req, int status)
{
    HttpServer* server = ((write_response_req*) req)->server;
    if (status < 0)
    {
        Logger::getInstance().error(std::string("Write error: ") + uv_strerror(status) + "\n");
        // error!
    }
    delete (std::vector<char>*) req->data;
    delete (write_response_req*) req;
}

static void __onNewConnection(uv_stream_t* tcp, int status)
{
    HttpServer* server = (HttpServer*) tcp->data;
    if (status < 0)
    {
        Logger::getInstance().error(std::string("New connection error: ") + uv_strerror(status));
        return;
    }
    uv_tcp_t* client = new uv_tcp_t;
    uv_tcp_init(uv_default_loop(), client);
    client->data = new Request(server, client);
    if (uv_accept(tcp, (uv_stream_t*) client) == 0)
    {
        uv_read_start((uv_stream_t*) client, __allocBuffer, __readCallback);
    }
    else
    {
        uv_close((uv_handle_t*) client, __closeCallback);
    }
}

void HttpServer::writeResponse(uv_stream_t* client, const Response& resp)
{
    std::vector<char>* buf_vec = new std::vector<char>;
    std::stringstream status_line;
    status_line << resp.httpVersion << " " << (int) resp.statusCode << " " << resp.reasonPhrase << "\r\n";
    std::string str = status_line.str();
    buf_vec->insert(buf_vec->end(), str.begin(), str.end());
    for (auto& e : resp.headers)
    {
        std::stringstream header;
        header << e.first << ": " << e.second << "\r\n";
        str = header.str();
        buf_vec->insert(buf_vec->end(), str.begin(), str.end());
    }
    if (resp.headers.find("Content-Length") == resp.headers.end())
    {
        std::stringstream header;
        header << "Content-Length: " << resp.body.size() << "\r\n";
        str = header.str();
        buf_vec->insert(buf_vec->end(), str.begin(), str.end());
    }
    buf_vec->insert(buf_vec->end(), {'\r', '\n'});
    buf_vec->insert(buf_vec->end(), resp.body.begin(), resp.body.end());
    delete &resp;
    write_response_req* req = new write_response_req;
    req->wt.data = buf_vec;
    req->server = this;
    uv_buf_t buf = uv_buf_init(buf_vec->data(), buf_vec->size());
    uv_write((uv_write_t*) req, client, &buf, 1, __writeCallback);
}

void HttpServer::start()
{
    std::stringstream ss;
    ss << "Start listening " << bindAddr << " port " << port << "\n";
    Logger::getInstance().info(ss.str());
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, __onNewConnection);
    if (r)
    {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        exit(1);
    }
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void HttpServer::process(const Request& req)
{
    Response* resp = new Response(req.getHttpVersion());
    middleware->call(req, *resp);
    uv_tcp_t* client = req.client;
    delete &req;
    writeResponse((uv_stream_t*) client, *resp);
}

struct fs_write_req
{
    uv_fs_t wt;
    uv_buf_t buf;
};

static void __write_log_callback(uv_fs_t* req)
{
    if (req->result < 0)
    {
        fprintf(stderr, "Write log error: %s\n", uv_strerror((int) req->result));
    }
    fs_write_req* write_req = (fs_write_req*) req;
    delete[] write_req->buf.base;
    delete req;
}
