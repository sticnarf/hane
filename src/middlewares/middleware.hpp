#ifndef HANE_MIDDLEWARE_HPP
#define HANE_MIDDLEWARE_HPP

#include <memory>
#include "../http/request/request.hpp"
#include "../http/response/response.hpp"

class Middleware {
public:
    virtual void call(const Request &req, std::shared_ptr<Response> resp) = 0;

    virtual ~Middleware() = default;
};

#endif
