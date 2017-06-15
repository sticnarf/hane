#ifndef RACKCPP_HTTP_H
#define RACKCPP_HTTP_H

#include <string>
#include <map>
#include <uv.h>
#include "request/request.h"
#include "response.h"

class Middleware;

class HttpServer
{
    Middleware* middleware;
    std::string bindAddr;
    int port;
    uv_tcp_t server;
    sockaddr_in addr;

    const int DEFAULT_BACKLOG = 128;

    void writeResponse(uv_stream_t* client, const Response& resp);

public:
    HttpServer(Middleware* middleware, const std::string _bindAddr, int port);

    ~HttpServer();

    void start();

    void process(const Request& req);
};

#endif
