#include "static_middleware.hpp"
#include <fstream>

void StaticMiddleware::call(const Request &req, std::shared_ptr<Response> resp) {
    std::fstream fs(path, std::ios::binary | std::ios::in);
    if (!fs.is_open()) {
        resp->setStatusCode(StatusCode::HTTP_NOT_FOUND);
        resp->body = "404 Not Found";
    } else {
        char buf[4096];
        while (fs.read(buf, sizeof(buf))) {
            auto len = fs.gcount();
            resp->body.insert(resp->body.end(), buf, buf + len);
        }
        auto len = fs.gcount();
        resp->body.insert(resp->body.end(), buf, buf + len);
    }
}

StaticMiddleware::StaticMiddleware(const std::string &path) : path(path) {}
