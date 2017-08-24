#include "client.hpp"
#include "errors.hpp"

/* TODO
 * Process (or even parse) requests in a separate thread.
 * Only use the main event loop for transfer
 */

//static void awaitRequest(uv_idle_t *handle) {
//    auto *client = static_cast<Client *>(handle->data);
//    client->processRequest();
//}

Client::Client(HttpServer *server)
        : server(server), queued(0) {
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

    // If there is no chunk in queue, the next request will be processed immediately.
    // Otherwise, the process of the next request should be triggered by the chunk-writing callback.
    if (queued == 0) {
        RequestPtr req = parser.yieldRequest();
        server->process(req, this->tcp);
    }
}
