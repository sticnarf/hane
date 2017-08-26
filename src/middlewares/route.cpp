#include <cassert>
#include "../http/errors.hpp"
#include "../utils/protocol_helper.hpp"
#include "../utils/string_utils.hpp"
#include "route.hpp"
#include "lambda_middleware.hpp"

MiddlewarePtr RouteMiddleware::call(RequestPtr req, std::shared_ptr<Response> &resp) {
    for (auto rule:rules[req->getHttpMethod()]) {
        auto regex = rule.first;
        if (std::regex_match(req->getAbsPath(), regex)) {
            return rule.second->call(req, resp);
        }
    }
    throw HttpError(StatusCode::HTTP_NOT_FOUND, "No rule found");
}

void RouteMiddleware::addRule(std::regex regex, HttpMethod method, std::shared_ptr<Middleware> middleware) {
    rules[method].emplace_back(regex, middleware);
}

RouteBuilder RouteMiddleware::operator()(HttpMethod method) {
    return RouteBuilder(std::dynamic_pointer_cast<RouteMiddleware>(shared_from_this()), method);
}

RouteBuilder RouteMiddleware::operator()(const std::string &method) {
    auto _method = StringUtils::toUppercase(method);
    return RouteBuilder(std::dynamic_pointer_cast<RouteMiddleware>(shared_from_this()),
                        toHttpMethod(_method.data(), _method.length()));
}

RouteBuilder::RouteBuilder(std::shared_ptr<RouteMiddleware> middleware, HttpMethod method) : middleware(
        middleware), method(method) {}

void RouteBuilder::operator()(std::function<void(RequestPtr, ResponsePtr)> function) {
    for (auto regex:regexes) {
        middleware->addRule(regex, method, std::make_shared<LambdaMiddleware>(function));
    }
}

void RouteBuilder::operator()(std::function<MiddlewarePtr(RequestPtr, std::shared_ptr<Response> &)> function) {
    for (auto regex:regexes) {
        middleware->addRule(regex, method, std::make_shared<LambdaMiddleware>(function));
    }
}

void RouteBuilder::operator()(MiddlewarePtr middleware) {
    for (auto regex:regexes) {
        this->middleware->addRule(regex, method, middleware);
    }
}

RouteBuilder &RouteBuilder::operator()(std::regex regex) {
    regexes.push_back(regex);
    return *this;
}

RouteBuilder &RouteBuilder::operator()(const std::string &regex) {
    regexes.push_back(std::regex(regex));
    return *this;
}