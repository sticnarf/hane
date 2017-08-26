#ifndef HANE_RESPONSE_HPP
#define HANE_RESPONSE_HPP

#include <string>
#include <map>
#include <vector>
#include <memory>
#include <uv.h>
#include "../header/cookies.hpp"
#include "../header/header.hpp"
#include "../../constants.hpp"

class Response {
    friend class HttpServer;

public:
    Response(const Response &resp) = default;

    explicit Response(HttpVersion version);

    virtual ~Response();

    void setStatusCode(StatusCode statusCode);

    bool isChunked() const;

    void makeChunked();

    void redirectTo(const std::string &location);

    Header headers;
    std::string body;
    Cookies cookies;

protected:
    HttpVersion httpVersion;
    StatusCode statusCode;
};

typedef std::shared_ptr<Response> ResponsePtr;

#endif
