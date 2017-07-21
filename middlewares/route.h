#ifndef RACKCPP_ROUTE_MIDDLEWARE_H
#define RACKCPP_ROUTE_MIDDLEWARE_H

#include "middleware.h"

namespace middleware
{
class Route : public Middleware
{
public:
    Route(std::shared_ptr<Middleware> nextMiddleware);

    void call(const Request& req, std::shared_ptr<Response> resp) override;

    void process(const Request& req, std::shared_ptr<Response> resp) override;
};
}

#endif
