﻿#ifndef HANE_MIDDLEWARE_HPP
#define HANE_MIDDLEWARE_HPP

#include <memory>
#include "../http/request/request.hpp"
#include "../http/response/response.hpp"

class Middleware;

typedef std::shared_ptr<Middleware> MiddlewarePtr;

class Middleware : public std::enable_shared_from_this<Middleware> {
public:
    virtual MiddlewarePtr call(const Request &req, std::shared_ptr<Response> resp) = 0;
};

#endif
