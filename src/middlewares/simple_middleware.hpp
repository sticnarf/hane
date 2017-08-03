#ifndef HANE_SIMPLE_MIDDLEWARE_HPP
#define HANE_SIMPLE_MIDDLEWARE_HPP

#include "middleware.hpp"

class SimpleMiddleware : public Middleware {
public:
    virtual void call(const Request &req, std::shared_ptr<Response> resp);

    virtual void process(const Request &req, std::shared_ptr<Response> resp) = 0;

    virtual ~SimpleMiddleware() = default;

protected:
    explicit SimpleMiddleware(std::shared_ptr<Middleware> nextMiddleware);

    std::shared_ptr<Middleware> nextMiddleware;
};

#endif
