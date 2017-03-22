#ifndef RACKCPP_RESPONSE_H
#define RACKCPP_RESPONSE_H

#include <string>
#include <unordered_map>
#include <vector>
#include <uv.h>

class Response {
    friend class HttpServer;
public:
    // See also: https://tools.ietf.org/html/rfc7231#section-6.1
    enum class StatusCode {
        HTTP_CONTINUE = 100,
        HTTP_SWITCHING_PROTOCOLS_CODE = 101,
        HTTP_OK = 200,
        HTTP_CREATED = 201,
        HTTP_ACCEPTED = 202,
        HTTP_NON_AUTHORITATIVE_INFORMATION = 203,
        HTTP_NO_CONTENT = 204,
        HTTP_RESET_CONTENT = 205,
        HTTP_PARTIAL_CONTENT = 206,
        HTTP_MULTIPLE_CHOICES = 300,
        HTTP_MOVED_PERMANENTLY = 301,
        HTTP_FOUND = 302,
        HTTP_SEE_OTHER = 303,
        HTTP_NOT_MODIFIED = 304,
        HTTP_USE_PROXY = 305,
        HTTP_TEMPORARY_REDIRECT = 307,
        HTTP_BAD_REQUEST = 400,
        HTTP_UNAUTHORIZED = 401,
        HTTP_PAYMENT_REQUIRED = 402,
        HTTP_FORBIDDEN = 403,
        HTTP_NOT_FOUND = 404,
        HTTP_METHOD_NOT_ALLOWED = 405,
        HTTP_NOT_ACCEPTABLE = 406,
        HTTP_PROXY_AUTHENTICATION_REQUIRED = 407,
        HTTP_REQUEST_TIMEOUT = 408,
        HTTP_CONFLICT = 409,
        HTTP_GONE = 410,
        HTTP_LENGTH_REQUIRED = 411,
        HTTP_PRECONDITION_FAILED = 412,
        HTTP_PAYLOAD_TOO_LARGE = 413,
        HTTP_URI_TOO_LONG = 414,
        HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
        HTTP_RANGE_NOT_SATISFIABLE = 416,
        HTTP_EXPECTATION_FAILED = 417,
        HTTP_UPGRADED_REQUIRED = 426,
        HTTP_INTERNAL_SERVER_ERROR = 500,
        HTTP_NOT_IMPLEMENTED = 501,
        HTTP_BAD_GATEWAY = 502,
        HTTP_SERVICE_UNAVAILABLE = 503,
        HTTP_GATEWAY_TIMEOUT = 504,
        HTTP_HTTP_VERSION_NOT_SUPPORTED = 505
    };

    Response(const std::string &http_version);
    ~Response();

    void set_status_code(StatusCode status_code);

    std::unordered_map<std::string, std::string> headers;
    std::vector<char> body;

private:
    std::string http_version;
    StatusCode status_code;
    std::string reason_phrase;
};

#endif
