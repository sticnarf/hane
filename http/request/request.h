#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H
#include <string>
#include "constants.h"
class Request
{
private:
    HttpVersion httpVersion;
    HttpMethod httpMethod;
    std::string target;

    // Set all parser classes as friends
    friend class AbstractParser;
    friend class StartLineParser;
};
#endif
