#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H

#include <string>
#include <vector>
#include <map>
#include <uv.h>
#include "request.fwd.h"
#include "http.h"

class Request {
    uv_tcp_t *client;
    HttpServer::Method method;
    std::string request_target;
    std::string http_version;
    std::map<std::string, std::string> headers;
    std::vector<char> body;

    class Parser {
        enum class Stage {
            REQUEST_LINE,
            HEADER_FIELDS,
            MESSAGE_BODY
        } stage = Stage::REQUEST_LINE;
        std::string buf;
        size_t buf_pos = 0;

        void parse_request_line(Request &req);

        void parse_header_fields(Request &req);

        void parse_message_body(Request &req);

    public:
        void push_buf(const char *buf, size_t len);

        void parse(Request &req);
    } parser;

public:
    Request(uv_tcp_t *client);

    void push_buf(const uv_buf_t *buf, ssize_t nread);
};

#endif