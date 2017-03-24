#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H

#include <string>
#include <vector>
#include <map>
#include <uv.h>
#include "method.h"
#include "header.h"

class HttpServer;

class Request {
    friend class HttpServer;

    HttpServer *http_server;
    uv_tcp_t *client;
    Method method;
    std::string request_target;
    std::string http_version;
    Header header;

private:
    std::map<std::string, std::string> queries;
    std::string body;

    class Parser {
        void parse_request_line(Request &req);

        void parse_url_queries(Request &req);

        void parse_queries(Request &req, const std::string &query_text);

        void parse_header_fields(Request &req);

        void parse_message_body(Request &req);

        void process_body(Request &req);

    public:
        enum class Stage {
            REQUEST_LINE,
            HEADER_FIELDS,
            MESSAGE_BODY,
            BODY_PROCESSING,
            PARSING_FINISHED
        } stage = Stage::REQUEST_LINE;
        std::string buf;
        size_t buf_pos = 0;

        void push_buf(const char *buf, size_t len);

        void parse(Request &req);
    } parser;

public:
    Request(HttpServer *http_server, uv_tcp_t *client);

    void push_buf(const uv_buf_t *buf, ssize_t nread);

    void process();

    void info_log(const std::string &msg);

    void error_log(const std::string &msg);

    bool is_info_log_enabled() const;

    bool is_error_log_enabled() const;

    Method get_method() const;

    const std::string &get_request_target() const;

    const std::string &get_http_version() const;

    const Header &get_header() const;

    const std::map<std::string, std::string> &get_queries() const;

    const std::string &get_body() const;

    ~Request();
};

#endif
