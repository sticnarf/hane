#ifndef RACKCPP_MIDDLEWARE_H
#define RACKCPP_MIDDLEWARE_H

#include "response.h"

class Request;

namespace middleware {
    class Middleware {
    public:
        virtual void call(const Request &req, Response &resp);

        virtual void process(const Request &req, Response &resp) = 0;

        virtual ~Middleware() {}

    protected:
        Middleware *next_middleware;
    };
}

#endif
