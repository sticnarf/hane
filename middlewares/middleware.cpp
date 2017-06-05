#include "middleware.h"

void Middleware::call(const Request &req, Response &resp)
{
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}
