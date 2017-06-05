#ifndef RACKCPP_MIDDLEWARE_H
#define RACKCPP_MIDDLEWARE_H

class Request;

class Response;

class Middleware {
public:
    virtual void call(const Request &req, Response &resp);

    virtual void process(const Request &req, Response &resp) = 0;

    virtual ~Middleware() {}

protected:
    Middleware *nextMiddleware;
};

#endif
