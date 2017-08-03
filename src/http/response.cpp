#include"response.hpp"

void Response::setStatusCode(StatusCode statusCode) {
    static std::map<StatusCode, std::string> reason_mapping = {
            {StatusCode::HTTP_CONTINUE,                      "Continue"},
            {StatusCode::HTTP_SWITCHING_PROTOCOLS_CODE,      "Switching Protocols"},
            {StatusCode::HTTP_OK,                            "OK"},
            {StatusCode::HTTP_CREATED,                       "Created"},
            {StatusCode::HTTP_ACCEPTED,                      "Accepted"},
            {StatusCode::HTTP_NON_AUTHORITATIVE_INFORMATION, "Non-Authoritative Information"},
            {StatusCode::HTTP_NO_CONTENT,                    "No Content"},
            {StatusCode::HTTP_RESET_CONTENT,                 "Reset Content"},
            {StatusCode::HTTP_PARTIAL_CONTENT,               "Partial Content"},
            {StatusCode::HTTP_MULTIPLE_CHOICES,              "Multiple Choices"},
            {StatusCode::HTTP_MOVED_PERMANENTLY,             "Moved Permanently"},
            {StatusCode::HTTP_FOUND,                         "Found"},
            {StatusCode::HTTP_SEE_OTHER,                     "See Other"},
            {StatusCode::HTTP_NOT_MODIFIED,                  "Not Modified"},
            {StatusCode::HTTP_USE_PROXY,                     "Use Proxy"},
            {StatusCode::HTTP_TEMPORARY_REDIRECT,            "Temporary Redirect"},
            {StatusCode::HTTP_BAD_REQUEST,                   "Bad Request"},
            {StatusCode::HTTP_UNAUTHORIZED,                  "Unauthorized"},
            {StatusCode::HTTP_PAYMENT_REQUIRED,              "Payment Required"},
            {StatusCode::HTTP_FORBIDDEN,                     "Forbidden"},
            {StatusCode::HTTP_NOT_FOUND,                     "Not Found"},
            {StatusCode::HTTP_METHOD_NOT_ALLOWED,            "Method Not Allowed"},
            {StatusCode::HTTP_NOT_ACCEPTABLE,                "Not Acceptable"},
            {StatusCode::HTTP_PROXY_AUTHENTICATION_REQUIRED, "Proxy Authentication Required"},
            {StatusCode::HTTP_REQUEST_TIMEOUT,               "Request Timeout"},
            {StatusCode::HTTP_CONFLICT,                      "Conflict"},
            {StatusCode::HTTP_GONE,                          "Gone"},
            {StatusCode::HTTP_LENGTH_REQUIRED,               "Length Required"},
            {StatusCode::HTTP_PRECONDITION_FAILED,           "Precondition Failed"},
            {StatusCode::HTTP_PAYLOAD_TOO_LARGE,             "Payload Too Large"},
            {StatusCode::HTTP_URI_TOO_LONG,                  "URI Too Long"},
            {StatusCode::HTTP_UNSUPPORTED_MEDIA_TYPE,        "Unsupported Media Type"},
            {StatusCode::HTTP_RANGE_NOT_SATISFIABLE,         "Range Not Satisfiable"},
            {StatusCode::HTTP_EXPECTATION_FAILED,            "Expectation Failed"},
            {StatusCode::HTTP_UPGRADED_REQUIRED,             "Upgrade Required"},
            {StatusCode::HTTP_INTERNAL_SERVER_ERROR,         "Internal Server Error"},
            {StatusCode::HTTP_NOT_IMPLEMENTED,               "Not Implemented"},
            {StatusCode::HTTP_BAD_GATEWAY,                   "Bad Gateway"},
            {StatusCode::HTTP_SERVICE_UNAVAILABLE,           "Service Unavailable"},
            {StatusCode::HTTP_GATEWAY_TIMEOUT,               "Gateway Timeout"},
            {StatusCode::HTTP_HTTP_VERSION_NOT_SUPPORTED,    "HTTP Version Not Supported"}
    };
    this->statusCode = statusCode;
    this->reasonPhrase = reason_mapping[statusCode];
}

Response::Response(HttpVersion version)
        : httpVersion(version), statusCode(StatusCode::HTTP_ACCEPTED) {}

Response::~Response() = default;
