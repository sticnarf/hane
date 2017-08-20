#ifndef HANE_ROUTE_MIDDLEWARE_HPP
#define HANE_ROUTE_MIDDLEWARE_HPP

#include "middleware.hpp"
#include <regex>
#include <utility>
#include <vector>

typedef std::pair<std::regex, std::shared_ptr<Middleware>> Rule;

class RouteMiddleware : public Middleware {
    std::vector<Rule> rules;
public:
    MiddlewarePtr call(Request &req, std::shared_ptr<Response> &resp) override;
    void addRule(std::regex, std::shared_ptr<Middleware>);
};

#endif
