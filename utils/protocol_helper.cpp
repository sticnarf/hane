#include <stdexcept>
#include <cstring>
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
    } else {
        throw std::invalid_argument("Unknown httpMethod");
    }
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
    } else {
        throw std::invalid_argument("Unknown HTTP httpVersion");
    }
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
