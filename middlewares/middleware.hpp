#ifndef HANE_MIDDLEWARE_HPP
#define HANE_MIDDLEWARE_HPP

#include <memory>

class Request;

class Response;

class Middleware {
public:
    virtual void call(const Request &req, std::shared_ptr<Response> resp) = 0;

    virtual ~Middleware() = default;
};

#endif
