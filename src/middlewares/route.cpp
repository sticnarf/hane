#include "../http/errors.hpp"
#include "route.hpp"

MiddlewarePtr RouteMiddleware::call(Request &req, std::shared_ptr<Response> &resp) {
    for (auto rule:rules[req.getHttpMethod()]) {
        auto regex = rule.first;
        if (std::regex_match(req.getAbsPath(), regex)) {
            return rule.second->call(req, resp);
        }
    }
    throw HttpError(StatusCode::HTTP_NOT_FOUND, "No rule found");
}

void RouteMiddleware::addRule(std::regex regex, HttpMethod method, std::shared_ptr<Middleware> middleware) {
    rules[method].emplace_back(regex, middleware);
}
