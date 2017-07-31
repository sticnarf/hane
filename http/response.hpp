#ifndef HANE_RESPONSE_HPP
#define HANE_RESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include <uv.h>
#include "constants.hpp"

class Response {
    friend class HttpServer;

public:
    explicit Response(HttpVersion version);

    ~Response();

    void setStatusCode(StatusCode statusCode);

    std::map<std::string, std::string> headers;
    std::string body;

private:
    HttpVersion httpVersion;
    StatusCode statusCode;
    std::string reasonPhrase;
};

#endif
