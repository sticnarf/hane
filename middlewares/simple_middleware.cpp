#include "simple_middleware.hpp"

void SimpleMiddleware::call(const Request &req, std::shared_ptr<Response> resp) {
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}

SimpleMiddleware::SimpleMiddleware(std::shared_ptr<Middleware> nextMiddleware)
        : nextMiddleware(nextMiddleware) {}
