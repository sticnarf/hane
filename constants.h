#ifndef RACKCPP_CONSTANTS_H
#define RACKCPP_CONSTANTS_H

enum class HttpVersion {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2
};

enum class HttpMethod {
    GET,
    HEAD,
    POST,
    PUT,
    DELETE,
    CONNECT,
    OPTIONS,
    TRACE
};

#endif
