#ifndef RACKCPP_MIDDLEWARE_H
#define RACKCPP_MIDDLEWARE_H

#include <memory>

class Request;

class Response;

class Middleware
{
public:
    virtual void call(const Request& req, std::shared_ptr<Response> resp);

    virtual void process(const Request& req, std::shared_ptr<Response> resp) = 0;

    virtual ~Middleware() = default;

protected:
    Middleware(std::shared_ptr<Middleware>);
    std::shared_ptr<Middleware> nextMiddleware;
};

#endif
