#ifndef HANE_ROUTE_MIDDLEWARE_HPP
#define HANE_ROUTE_MIDDLEWARE_HPP

#include "middleware.hpp"
#include <regex>
#include <utility>
#include <vector>
#include <map>

typedef std::pair<std::regex, std::shared_ptr<Middleware>> Rule;

class RouteMiddleware : public Middleware {
    std::map<HttpMethod, std::vector<Rule>> rules;
public:
    MiddlewarePtr call(RequestPtr req, std::shared_ptr<Response> &resp) override;

    void addRule(std::regex, HttpMethod, std::shared_ptr<Middleware>);
};

#endif
