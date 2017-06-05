#ifndef RACKCPP_HELLO_H
#define RACKCPP_HELLO_H

#include "middlewares/middleware.h"

namespace middleware {
    class Hello : public Middleware {
    public:
        Hello(Middleware *next_middleware);

        void call(const Request &req, Response &resp) override;

        void process(const Request &req, Response &resp) override;

        ~Hello();
    };
}

#endif
