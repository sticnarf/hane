#include "simple_middleware.hpp"

#include <utility>

MiddlewarePtr SimpleMiddleware::call(Request &req, std::shared_ptr<Response> &resp) {
    process(req, resp);
    if (nextMiddleware)
        return nextMiddleware->call(req, resp);
    return nullptr;
}

SimpleMiddleware::SimpleMiddleware(MiddlewarePtr nextMiddleware)
        : nextMiddleware(std::move(nextMiddleware)) {}

void SimpleMiddleware::process(Request &req, std::shared_ptr<Response> &resp) {
    // empty implementation
}
