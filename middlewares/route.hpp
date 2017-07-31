#ifndef RACKCPP_ROUTE_MIDDLEWARE_HPP
#define RACKCPP_ROUTE_MIDDLEWARE_HPP

#include "middleware.hpp"

class RouteMiddleware : public Middleware {
public:
    void call(const Request &req, std::shared_ptr<Response> resp) override;


};

#endif
