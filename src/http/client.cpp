#include "client.hpp"

/* TODO
 * Process (or even parse) requests in a separate thread.
 * Only use the main event loop for transfer
 */

//static void awaitRequest(uv_idle_t *handle) {
//    auto *client = static_cast<Client *>(handle->data);
//    client->processRequest();
//}

Client::Client(HttpServer *server)
        : server(server) {
    tcp = new uv_tcp_t;
    tcp->data = this;
}

void Client::pushBuf(const char *buf, size_t len) {
    parser.pushBuf(buf, len);

    processRequest();
}

Client::~Client() {
    delete tcp;
}

void Client::processRequest() {
    if (!parser.hasCompleteRequest())
        return;

    Request req = parser.yieldRequest();
    server->process(req, this->tcp);
}
