#include "client.h"

Client::Client(HttpServer* server, uv_tcp_t* tcp)
    :server(server), tcp(tcp) { }

void Client::push_buf(const char* buf, size_t len)
{

}
