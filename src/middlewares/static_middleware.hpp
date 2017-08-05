#ifndef HANE_STATIC_MIDDLEWARE_HPP
#define HANE_STATIC_MIDDLEWARE_HPP

#include "middleware.hpp"
#include <string>

class StaticMiddleware : public Middleware {
    std::string path;
public:
    explicit StaticMiddleware(const std::string &path);

    void call(const Request &req, std::shared_ptr<Response> resp) override;
};

#endif
