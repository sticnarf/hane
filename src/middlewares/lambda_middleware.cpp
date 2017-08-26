#include "lambda_middleware.hpp"

#include <utility>

LambdaMiddleware::LambdaMiddleware(std::function<void(RequestPtr, ResponsePtr)> simpleFunction)
        : simpleFunction(std::move(simpleFunction)) {
}

MiddlewarePtr LambdaMiddleware::call(RequestPtr req, std::shared_ptr<Response> &resp) {
    if (complexFunction)
        return complexFunction(req, resp);

    if (simpleFunction)
        simpleFunction(req, resp);

    return nullptr;
}

LambdaMiddleware::LambdaMiddleware(
        std::function<MiddlewarePtr(RequestPtr, std::shared_ptr<Response> &)> complexFunction)
        : complexFunction(std::move(complexFunction)) {
}
