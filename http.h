#pragma once

#include <string>
#include <unordered_map>
#include "middleware.h"

using namespace middleware;

class HttpServer {
    Middleware *middleware;
    std::string bind_addr;
    int port;
    uv_tcp_t server;
    sockaddr_in addr;

    const int DEFAULT_BACKLOG = 128;

public:
    HttpServer(Middleware *middleware, const std::string bind_addr, int port);

    ~HttpServer();

    void start();

    // See also: https://tools.ietf.org/html/rfc7231#section-4
    enum class Method {
        HTTP_GET,
        HTTP_HEAD,
        HTTP_POST,
        HTTP_PUT,
        HTTP_DELETE,
        HTTP_CONNECT,
        HTTP_OPTIONS,
        HTTP_TRACE
    };

    static Method parse_method(const std::string &string_method) {
        static std::unordered_map<std::string, Method> mapping = {
                {"GET",     Method::HTTP_GET},
                {"HEAD",    Method::HTTP_HEAD},
                {"POST",    Method::HTTP_POST},
                {"PUT",     Method::HTTP_PUT},
                {"DELETE",  Method::HTTP_DELETE},
                {"CONNECT", Method::HTTP_CONNECT},
                {"OPTIONS", Method::HTTP_OPTIONS},
                {"TRACE",   Method::HTTP_TRACE}
        };
        return mapping[string_method];
    }
};