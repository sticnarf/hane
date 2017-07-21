#ifndef RACKCPP_HTTP_H
#define RACKCPP_HTTP_H

#include <string>
#include <map>
#include <memory>
#include <uv.h>
#include "request/request.h"
#include "response.h"

class Middleware;

class HttpServer
{
    std::unique_ptr<Middleware> middleware;
    std::string bindAddr;
    int port;
    uv_tcp_t server;
    sockaddr_in addr;

    const int DEFAULT_BACKLOG = 128;

    void writeResponse(uv_stream_t* client, std::shared_ptr<const Response> resp);

public:
    HttpServer(std::unique_ptr<Middleware>&& middleware, const std::string& _bindAddr, int port);

    ~HttpServer();

    void start();

    void process(const Request& req, uv_tcp_t* client);
};

#endif
