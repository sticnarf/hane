#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H
#include <string>
#include "constants.h"
#include "header.h"
#include "utils/buffer.h"

typedef Buffer Body;

class Request
{
private:
    HttpVersion httpVersion;
    HttpMethod httpMethod;
    std::string target;

    Header header;
    Body body;


    // Set all parser classes as friends
    friend class AbstractParser;
    friend class StartLineParser;
    friend class HeaderParser;
    friend class SizedBodyParser;
    friend class FinalParser;
};
#endif
