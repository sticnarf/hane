#ifndef HANE_SIMPLE_MIDDLEWARE_HPP
#define HANE_SIMPLE_MIDDLEWARE_HPP

#include "middleware.hpp"

class SimpleMiddleware : public Middleware {
public:
    virtual MiddlewarePtr call(Request &req, std::shared_ptr<Response> &resp);

    virtual void process(Request &req, std::shared_ptr<Response> &resp) = 0;

    virtual ~SimpleMiddleware() = default;

protected:
    explicit SimpleMiddleware(MiddlewarePtr nextMiddleware);

    std::shared_ptr<Middleware> nextMiddleware;
};

#endif
