#ifndef RACKCPP_CLIENT_H
#define RACKCPP_CLIENT_H
#include "http/parser/parser.h"
#include "http.h"

class Client
{
private:
    HttpServer* server;
    uv_tcp_t* tcp;
    Parser parser;
public:
    Client(HttpServer* server, uv_tcp_t* tcp);
    void push_buf(const char* buf, size_t len);
};

#endif
