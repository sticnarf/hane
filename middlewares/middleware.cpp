#include "middleware.h"

void Middleware::call(const Request& req, std::shared_ptr<Response> resp)
{
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}
