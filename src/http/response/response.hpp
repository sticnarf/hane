#ifndef HANE_RESPONSE_HPP
#define HANE_RESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <uv.h>
#include "../header/header.hpp"
#include "../../constants.hpp"

class Response {
    friend class HttpServer;

public:
    Response(const Response &resp) = default;

    explicit Response(HttpVersion version);

    ~Response();

    void setStatusCode(StatusCode statusCode);

    bool isChunked();

    Header headers;
    std::string body;

protected:
    HttpVersion httpVersion;
    StatusCode statusCode;
};

typedef std::shared_ptr<Response> ResponsePtr;

#endif
