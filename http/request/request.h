#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H
#include <string>
#include "constants.h"
class Request
{
private:
    HttpVersion version;
    HttpMethod method;
    std::string target;
};
#endif
