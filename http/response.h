#ifndef RACKCPP_RESPONSE_H
#define RACKCPP_RESPONSE_H

#include <string>
#include <map>
#include <vector>
#include <uv.h>
#include "constants.h"

class Response
{
    friend class HttpServer;

public:
    Response(HttpVersion version);

    ~Response();

    void setStatusCode(StatusCode statusCode);

    std::map<std::string, std::string> headers;
    std::vector<char> body;

private:
    HttpVersion httpVersion;
    StatusCode statusCode;
    std::string reasonPhrase;
};

#endif
