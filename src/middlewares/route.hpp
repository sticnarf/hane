#ifndef HANE_ROUTE_MIDDLEWARE_HPP
#define HANE_ROUTE_MIDDLEWARE_HPP

#include "middleware.hpp"
#include <regex>
#include <utility>
#include <vector>
#include <map>
#include <memory>
#include <functional>

typedef std::pair<std::regex, std::shared_ptr<Middleware>> Rule;

class RouteMiddleware;

class RouteBuilder {
    std::shared_ptr<RouteMiddleware> middleware;
    HttpMethod method;
    std::vector<std::regex> regexes;

public:
    RouteBuilder(std::shared_ptr<RouteMiddleware> middleware, HttpMethod method);

    RouteBuilder &operator()(std::regex);

    RouteBuilder &operator()(const std::string &);

    void operator()(std::function<void(RequestPtr, ResponsePtr)>);

    void operator()(std::function<MiddlewarePtr(RequestPtr, std::shared_ptr<Response> &)>);

    void operator()(MiddlewarePtr middleware);
};

class RouteMiddleware : public Middleware {
    std::map<HttpMethod, std::vector<Rule>> rules;
public:
    MiddlewarePtr call(RequestPtr req, std::shared_ptr<Response> &resp) override;

    void addRule(std::regex, HttpMethod, std::shared_ptr<Middleware>);

    RouteBuilder operator()(HttpMethod);

    RouteBuilder operator()(const std::string &);
};

#endif
