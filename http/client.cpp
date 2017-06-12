#include "client.h"

Client::Client(HttpServer* server, uv_tcp_t* tcp)
        :server(server), tcp(tcp) { }

void Client::pushBuf(const char* buf, size_t len)
{
    parser.pushBuf(buf, len);
}

void awaitRequest();
