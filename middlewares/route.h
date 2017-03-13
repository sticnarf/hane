#ifndef RACKCPP_ROUTE_MIDDLEWARE_H
#define RACKCPP_ROUTE_MIDDLEWARE_H

#include "../middleware.h"

namespace middleware {
    class Route : public Middleware {
    public:
        Route(Middleware *next_middleware);

        void call(const Request &req, Response &resp) override;

        ~Route();
    };
}

#endif