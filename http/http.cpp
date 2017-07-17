#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <uv.h>
#include <rackcpp.h>
#include "client.h"

HttpServer::HttpServer(Middleware* middleware, const std::string _bindAddr, int port)
    :middleware(middleware), bindAddr(_bindAddr), port(port)
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

static void closeCallback(uv_handle_t* handle)
{
    delete (Request*) handle->data;
    delete (uv_tcp_t*) handle;
}

static void allocBuffer(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf)
{
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

static void readCallback(uv_stream_t* clientTcp, ssize_t nread, const uv_buf_t* buf)
{
    Client* client = (Client*) clientTcp->data;
    if (nread > 0)
    {
        Logger::getInstance().info(std::string("Read ") + std::to_string(nread) + "bytes.\n");
        client->pushBuf(buf->base, nread);
    }
    if (nread < 0)
    {
        if (nread != UV_EOF)
            Logger::getInstance().error(std::string("Read error: ") + uv_strerror((int) nread) + "\n");
        uv_close((uv_handle_t*) clientTcp, closeCallback);
    }
    delete[] buf->base;
}

struct write_response_req
{
    uv_write_t wt;
    HttpServer* server;
};

static void writeCallback(uv_write_t* req, int status)
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

static void onNewConnection(uv_stream_t* serverTcp, int status)
{
    HttpServer* server = (HttpServer*) serverTcp->data;
    if (status < 0)
    {
        Logger::getInstance().error(std::string("New connection error: ") + uv_strerror(status));
        return;
    }
    uv_tcp_t* clientTcp = new uv_tcp_t;
    uv_tcp_init(uv_default_loop(), clientTcp);
//    clientTcp->data = new Request(server, clientTcp);
    clientTcp->data = new Client(server, clientTcp);
    if (uv_accept(serverTcp, (uv_stream_t*) clientTcp) == 0)
    {
        uv_read_start((uv_stream_t*) clientTcp, allocBuffer, readCallback);
    }
    else
    {
        uv_close((uv_handle_t*) clientTcp, closeCallback);
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
    uv_write((uv_write_t*) req, client, &buf, 1, writeCallback);
}

void HttpServer::start()
{
    std::stringstream ss;
    ss << "Start listening " << bindAddr << " port " << port << "\n";
    Logger::getInstance().info(ss.str());
    int r = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, onNewConnection);
    if (r)
    {
        Logger::getInstance().error(std::string("Listen error ") + uv_strerror(r) + "\n");
        exit(1);
    }
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void HttpServer::process(const Request& req)
{
//    Response* resp = new Response(req.getHttpVersion());
//    middleware->call(req, *resp);
//    uv_tcp_t* client = req.client;
    delete &req;
//    writeResponse((uv_stream_t*) client, *resp);
}
