#include "middleware.h"

void Middleware::call(const Request& req, std::shared_ptr<Response> resp)
{
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}

Middleware::Middleware(std::shared_ptr<Middleware> nextMiddleware)
        :nextMiddleware(nextMiddleware) { }
