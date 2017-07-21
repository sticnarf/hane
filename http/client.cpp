#include "client.h"

/* TODO
 * Process (or even parse) requests in a separate thread.
 * Only use the main event loop for transfer
 */

static void awaitRequest(uv_idle_t* handle)
{
    Client* client = (Client*) handle->data;
    client->processRequest();
}

Client::Client(HttpServer* server)
        :server(server)
{
    tcp = new uv_tcp_t;
    tcp->data = this;
//    idler.data = this;
//    uv_idle_init(uv_default_loop(), &idler);
//    uv_idle_start(&idler, awaitRequest);
}

void Client::pushBuf(const char* buf, size_t len)
{
    parser.pushBuf(buf, len);

    processRequest();
}

Client::~Client()
{
    delete tcp;
//     uv_idle_stop(&idler);
}

void Client::processRequest()
{
    if (!parser.hasCompleteRequest())
        return;

    Request req = parser.yieldRequest();
    server->process(req, this->tcp);
}
