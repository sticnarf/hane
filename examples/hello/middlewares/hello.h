#ifndef RACKCPP_HELLO_H
#define RACKCPP_HELLO_H

#include "middlewares/middleware.h"

namespace middleware
{
class Hello: public Middleware
{
public:
    Hello(std::shared_ptr<Middleware> next_middleware);

    void call(const Request& req, std::shared_ptr<Response> resp) override;

    void process(const Request& req, std::shared_ptr<Response> resp) override;
};
}

#endif
