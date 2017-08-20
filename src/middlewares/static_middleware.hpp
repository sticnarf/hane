#ifndef HANE_STATIC_MIDDLEWARE_HPP
#define HANE_STATIC_MIDDLEWARE_HPP

#include "middleware.hpp"
#include <string>
#include <fstream>

class StaticMiddleware : public Middleware {
    std::string path;
    std::fstream file;
public:
    explicit StaticMiddleware(const std::string &path);

    MiddlewarePtr call(Request &req, std::shared_ptr<Response> &resp) override;
};

#endif
