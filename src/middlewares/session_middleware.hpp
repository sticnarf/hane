#ifndef HANE_SESSION_MIDDLEWARE_HPP
#define HANE_SESSION_MIDDLEWARE_HPP

#include "simple_middleware.hpp"

class SessionMiddleware : public SimpleMiddleware {
public:
    explicit SessionMiddleware(MiddlewarePtr nextMiddleware);

    virtual void process(RequestPtr req, std::shared_ptr<Response> &resp);

    virtual ~SessionMiddleware() = default;

protected:
    std::shared_ptr<Middleware> nextMiddleware;
};

#endif
