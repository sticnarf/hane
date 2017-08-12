#include <stdexcept>
#include <cstring>
#include <map>
#include "protocol_helper.hpp"

HttpMethod toHttpMethod(const char *str, size_t len) {
    if (len < 2) {
        throw std::invalid_argument("Method length is too short");
    }

    HttpMethod method;
    switch (str[0]) {
        case 'G':
            method = HttpMethod::METHOD_GET;
            break;
        case 'H':
            method = HttpMethod::METHOD_HEAD;
            break;
        case 'D':
            method = HttpMethod::METHOD_DELETE;
            break;
        case 'C':
            method = HttpMethod::METHOD_CONNECT;
            break;
        case 'O':
            method = HttpMethod::METHOD_OPTIONS;
            break;
        case 'T':
            method = HttpMethod::METHOD_TRACE;
            break;
        default:
            method = str[1] == 'O' ? HttpMethod::METHOD_POST : HttpMethod::METHOD_PUT;
    }

    const char *validation = stringify(method);
    if (strlen(validation) == len && strncmp(str, validation, len) == 0) {
        return method;
    }
    throw std::invalid_argument("Unknown httpMethod");
}

const char *stringify(HttpMethod method) {
    switch (method) {
        case HttpMethod::METHOD_GET:
            return HTTP_METHOD_GET_STRING;
        case HttpMethod::METHOD_POST:
            return HTTP_METHOD_POST_STRING;
        case HttpMethod::METHOD_PUT:
            return HTTP_METHOD_PUT_STRING;
        case HttpMethod::METHOD_HEAD:
            return HTTP_METHOD_HEAD_STRING;
        case HttpMethod::METHOD_CONNECT:
            return HTTP_METHOD_CONNECT_STRING;
        case HttpMethod::METHOD_DELETE:
            return HTTP_METHOD_DELETE_STRING;
        case HttpMethod::METHOD_OPTIONS:
            return HTTP_METHOD_OPTIONS_STRING;
        case HttpMethod::METHOD_TRACE:
            return HTTP_METHOD_TRACE_STRING;
    }
}

HttpVersion toHttpVersion(const char *str, size_t len) {
    HttpVersion version;
    if (len == 6) {
        version = HttpVersion::HTTP_2;
    } else if (len == 8) {
        if (str[7] == '0') {
            version = HttpVersion::HTTP_1_0;
        } else {
            version = HttpVersion::HTTP_1_1;
        }
    } else {
        throw std::invalid_argument("Unknown HTTP httpVersion");
    }

    const char *validation = stringify(version);
    if (strncmp(validation, str, len) == 0) {
        return version;
    }
    throw std::invalid_argument("Unknown HTTP httpVersion");
}

const char *stringify(HttpVersion version) {
    switch (version) {
        case HttpVersion::HTTP_1_0:
            return HTTP_1_0_STRING;
        case HttpVersion::HTTP_1_1:
            return HTTP_1_1_STRING;
        case HttpVersion::HTTP_2:
            return HTTP_2_STRING;
    }
}

const char *toReasonPhrase(StatusCode code) {
    static std::map<StatusCode, const char *> reasonMapping = {
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

    return reasonMapping.find(code)->second;
}