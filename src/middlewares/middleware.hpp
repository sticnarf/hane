#ifndef HANE_MIDDLEWARE_HPP
#define HANE_MIDDLEWARE_HPP

#include <memory>
#include "../http/request/request.hpp"
#include "../http/response/response.hpp"
#include "../http/response/chunked_response.hpp"

class Middleware;

typedef std::shared_ptr<Middleware> MiddlewarePtr;

class Middleware : public std::enable_shared_from_this<Middleware> {
public:
    /**
     * If the call function returns nullptr, then the middleware finishes.
     * @param req
     * @param resp
     * @return The middleware that remains to be called
     */
    virtual MiddlewarePtr call(Request &req, std::shared_ptr<Response>& resp) = 0;
};

#endif
