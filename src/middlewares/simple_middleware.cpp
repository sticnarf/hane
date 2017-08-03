#include "simple_middleware.hpp"

#include <utility>

void SimpleMiddleware::call(const Request &req, std::shared_ptr<Response> resp) {
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}

SimpleMiddleware::SimpleMiddleware(std::shared_ptr<Middleware> nextMiddleware)
        : nextMiddleware(std::move(nextMiddleware)) {}
