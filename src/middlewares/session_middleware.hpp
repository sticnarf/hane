#ifndef HANE_SESSION_MIDDLEWARE_HPP
#define HANE_SESSION_MIDDLEWARE_HPP

#include "simple_middleware.hpp"

class SessionMiddleware : public SimpleMiddleware {
public:
    virtual void process(Request &req, std::shared_ptr<Response> &resp);

    virtual ~SessionMiddleware() = default;

protected:
    explicit SessionMiddleware(MiddlewarePtr nextMiddleware);

    std::shared_ptr<Middleware> nextMiddleware;
};

#endif
