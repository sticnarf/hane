#ifndef RACKCPP_CLIENT_HPP
#define RACKCPP_CLIENT_HPP

#include "http/parser/parser.hpp"
#include "http.hpp"

class Client {
private:
    HttpServer *server;
    uv_idle_t idler;
    uv_tcp_t *tcp;
    uv_write_t write;
    uv_buf_t buf;
    Parser parser;
public:
    Client(HttpServer *server);

    void pushBuf(const char *buf, size_t len);

    void processRequest();

    ~Client();

    friend class HttpServer;

    friend void onNewConnection(uv_stream_t *serverTcp, int status);
};

#endif