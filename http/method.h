#ifndef RACKCPP_METHOD_H
#define RACKCPP_METHOD_H

#include <string>

// See also: https://tools.ietf.org/html/rfc7231#section-4
enum class Method
{
    HTTP_GET,
    HTTP_HEAD,
    HTTP_POST,
    HTTP_PUT,
    HTTP_DELETE,
    HTTP_CONNECT,
    HTTP_OPTIONS,
    HTTP_TRACE
};

Method parseMethod(const std::string &stringMethod);

#endif
