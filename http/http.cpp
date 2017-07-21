#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <uv.h>
#include "utils/logger.h"
#include "utils/protocol_helper.h"
#include "client.h"
#include "middlewares/middleware.h"

HttpServer::HttpServer(std::unique_ptr<Middleware>&& middleware, const std::string& _bindAddr, int port)
        :middleware(std::move(middleware)), bindAddr(_bindAddr), port(port)
{
    uv_tcp_init(uv_default_loop(), &server);
    uv_ip4_addr(_bindAddr.c_str(), port, &addr);
    uv_tcp_bind(&server, (const sockaddr*) &addr, 0);
    server.data = this;
}

HttpServer::~HttpServer()
{
}

static void closeCallback(uv_handle_t* handle)
{
    printf("Close callback %p\n", handle);
    printf("Delete %p\n", handle->data);
    delete (Client*) handle->data;
//    printf("Delete handle %p\n", handle);
//    delete (uv_tcp_t*) handle;
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

static void writeCallback(uv_write_t* req, int status)
{
//    HttpServer* server = ((write_response_req*) req)->server;
    if (status < 0)
    {
        Logger::getInstance().error(std::string("Write error: ") + uv_strerror(status) + "\n");
        // error!
    }

    printf("Write OK!\n");
    delete[] (char*) req->data;
    printf("Clear req ok\n");
}

void onNewConnection(uv_stream_t* serverTcp, int status)
{
    HttpServer* server = (HttpServer*) serverTcp->data;
    if (status < 0)
    {
        Logger::getInstance().error(std::string("New connection error: ") + uv_strerror(status));
        return;
    }

    Client* client = new Client(server);
    printf("New connection %p\n", client->tcp);
    uv_tcp_init(uv_default_loop(), client->tcp);

    if (uv_accept(serverTcp, (uv_stream_t*) client->tcp) == 0)
    {
        uv_read_start((uv_stream_t*) client->tcp, allocBuffer, readCallback);
    }
    else
    {
        uv_close((uv_handle_t*) client->tcp, closeCallback);
    }
}

void HttpServer::writeResponse(uv_stream_t* tcp, std::shared_ptr<const Response> resp)
{
    std::stringstream responseText;

    // Append status line
    responseText << stringify(resp->httpVersion) << " "
                 << (int) resp->statusCode << " " << resp->reasonPhrase << "\r\n";

    // Append headers
    for (auto& e : resp->headers)
    {
        responseText << e.first << ": " << e.second << "\r\n";
    }

    // Append Content-Length
    if (resp->headers.find("Content-Length") == resp->headers.end())
    {
        responseText << "Content-Length: " << resp->body.size() << "\r\n";
    }

    // Append Body
    responseText << "\r\n" << resp->body;

    std::string str = responseText.str();

    auto data = new char[str.length()];
    memcpy(data, str.data(), str.length());

    Client* client = static_cast<Client*>(tcp->data);
    client->write.data = data;

    client->buf = uv_buf_init(data, str.length());
    printf("Write response! %p %p %p\n", &client->write, client, &client->buf);
    uv_write(&client->write, tcp, &client->buf, 1, writeCallback);
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

void HttpServer::process(const Request& req, uv_tcp_t* client)
{
    auto resp = std::make_shared<Response>(req.getHttpVersion());
    middleware->call(req, resp);
    writeResponse((uv_stream_t*) client, resp);
}
