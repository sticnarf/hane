#ifndef RACKCPP_CONSTANTS_H
#define RACKCPP_CONSTANTS_H

enum class HttpVersion {
    HTTP_1_0,
    HTTP_1_1,
    HTTP_2
};

const char* const HTTP_1_0_STRING = "HTTP/1.0";
const char* const HTTP_1_1_STRING = "HTTP/1.1";
const char* const HTTP_2_STRING = "HTTP/2";

enum class HttpMethod {
    METHOD_GET,
    METHOD_HEAD,
    METHOD_POST,
    METHOD_PUT,
    METHOD_DELETE,
    METHOD_CONNECT,
    METHOD_OPTIONS,
    METHOD_TRACE
};

const char* const HTTP_METHOD_GET_STRING = "GET";
const char* const HTTP_METHOD_HEAD_STRING = "HEAD";
const char* const HTTP_METHOD_POST_STRING = "POST";
const char* const HTTP_METHOD_PUT_STRING = "PUT";
const char* const HTTP_METHOD_DELETE_STRING = "DELETE";
const char* const HTTP_METHOD_CONNECT_STRING = "CONNECT";
const char* const HTTP_METHOD_OPTIONS_STRING = "OPTIONS";
const char* const HTTP_METHOD_TRACE_STRING = "TRACE";



#endif
