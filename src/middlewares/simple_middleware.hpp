#ifndef HANE_SIMPLE_MIDDLEWARE_HPP
#define HANE_SIMPLE_MIDDLEWARE_HPP

#include "middleware.hpp"

class SimpleMiddleware : public Middleware {
public:
    virtual MiddlewarePtr call(RequestPtr req, std::shared_ptr<Response> &resp);

    virtual void process(RequestPtr req, std::shared_ptr<Response> &resp);

    virtual ~SimpleMiddleware() = default;

protected:
    explicit SimpleMiddleware(MiddlewarePtr nextMiddleware);

    std::shared_ptr<Middleware> nextMiddleware;
};

#endif
