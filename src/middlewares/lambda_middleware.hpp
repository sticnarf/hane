#ifndef HANE_LAMBDA_MIDDLEWARE_HPP
#define HANE_LAMBDA_MIDDLEWARE_HPP

#include "middleware.hpp"

class LambdaMiddleware : public Middleware {
    std::function<void(RequestPtr, ResponsePtr)> simpleFunction;
    std::function<MiddlewarePtr(RequestPtr, std::shared_ptr<Response> &)> complexFunction;

public:
    virtual MiddlewarePtr call(RequestPtr req, std::shared_ptr<Response> &resp);

    LambdaMiddleware(std::function<void(RequestPtr, ResponsePtr)>);

    LambdaMiddleware(std::function<MiddlewarePtr(RequestPtr, std::shared_ptr<Response> &)>);
};

#endif
