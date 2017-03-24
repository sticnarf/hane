#include <string>
#include <cctype>
#include <uv.h>
#include "request.h"
#include "http.h"

Request::Request(HttpServer *http_server, uv_tcp_t *client) : http_server(http_server), client(client) {}

Request::~Request() {}

void Request::push_buf(const uv_buf_t *buf, ssize_t nread) {
    parser.push_buf(buf->base, nread);
    parser.parse(*this);
}

void Request::process() {
    auto new_req = new Request(http_server, client);
    if (parser.buf_pos < parser.buf.size()) {
        new_req->parser.push_buf(parser.buf.data() + parser.buf_pos, parser.buf.size() - parser.buf_pos);
    }
    client->data = new_req;
    http_server->process(*this);
}

Method Request::get_method() const {
    return method;
}

const std::map<std::string, std::string> &Request::get_queries() const {
    return queries;
}

const std::string &Request::get_request_target() const {
    return request_target;
}

const std::string &Request::get_http_version() const {
    return http_version;
}

const Header &Request::get_header() const {
    return header;
}

const std::string &Request::get_body() const {
    return body;
}

void Request::info_log(const std::string &msg) {
    http_server->info_log(msg);
}

void Request::error_log(const std::string &msg) {
    http_server->error_log(msg);
}

bool Request::is_info_log_enabled() const {
    return http_server->is_info_log_enabled();
}

bool Request::is_error_log_enabled() const {
    return http_server->is_error_log_enabled();
}

void Request::Parser::parse_request_line(Request &req) {
    size_t line_end = buf.find("\r\n", buf_pos);
    if (line_end != std::string::npos) {
        size_t sep = buf.find(' ', buf_pos);
        req.method = parse_method(buf.substr(buf_pos, (sep++) - buf_pos));
        size_t sep2 = buf.find(' ', sep);
        req.request_target = buf.substr(sep, (sep2++) - sep);
        parse_url_queries(req);
        req.http_version = buf.substr(sep2, line_end - sep2);
        buf_pos = line_end + 2;
        stage = Stage::HEADER_FIELDS;
        parse(req);
    }
}

void Request::Parser::parse_header_fields(Request &req) {
    size_t line_end = buf.find("\r\n", buf_pos);
    if (line_end == buf_pos) {
        buf_pos = line_end + 2;
        stage = Stage::MESSAGE_BODY;
        parse(req);
    } else if (line_end != std::string::npos) {
        size_t sep = buf.find(':', buf_pos);
        std::string field_name = buf.substr(buf_pos, (sep++) - buf_pos);
        while (isspace(buf[sep])) sep++;
        size_t value_end = line_end - 1;
        while (isspace(buf[value_end])) value_end--;
        std::string field_value = buf.substr(sep, value_end + 1 - sep);
        req.header.put(field_name, field_value);
        buf_pos = line_end + 2;
        parse(req);
    }
}

void Request::Parser::parse_message_body(Request &req) {
    switch (req.method) {
        case Method::HTTP_GET:
            stage = Stage::PARSING_FINISHED;
            break;
        default:
            auto transfer_encoding = req.header.get_value("Transfer-Encoding");
            if (transfer_encoding != req.header.end_iterator()) {
                // TODO: Transfer-Encoding is not supported
            } else {
                int content_length = stoi(req.header.get_value("Content-Length")->second);
                if (buf.size() - buf_pos >= content_length) {
                    req.body += buf.substr(buf_pos, content_length);
                    buf_pos += content_length;
                    stage = Stage::BODY_PROCESSING;
                }
            }
            break;
    }
    parse(req);
}

void Request::Parser::push_buf(const char *buf, size_t len) {
    this->buf.append(buf, len);
}

void Request::Parser::parse(Request &req) {
    switch (stage) {
        case Stage::REQUEST_LINE:
            parse_request_line(req);
            break;
        case Stage::HEADER_FIELDS:
            parse_header_fields(req);
            break;
        case Stage::MESSAGE_BODY:
            parse_message_body(req);
            break;
        case Stage::BODY_PROCESSING:
            process_body(req);
            break;
        case Stage::PARSING_FINISHED:
            req.process();
            break;
    }
}


void Request::Parser::parse_url_queries(Request &req) {
    const std::string &target = req.request_target;
    size_t begin = target.find('?');
    if (begin != std::string::npos) {
        parse_queries(req, target.substr(begin + 1));
    }
}

void Request::Parser::process_body(Request &req) {
    std::string content_type = req.header.get_value("Content-Type")->second;
    if (content_type.find("x-www-form-urlencoded") != std::string::npos) {
        parse_queries(req, req.body);
    }
    stage = Stage::PARSING_FINISHED;
    parse(req);
}

void Request::Parser::parse_queries(Request &req, const std::string &query_text) {
    size_t begin = 0;
    while (isspace(query_text[begin]))
        begin++;
    for (;;) {
        size_t equal_sign = query_text.find('=', begin);
        if (equal_sign == std::string::npos)
            break;
        std::string key, val;
        for (size_t i = begin; i < equal_sign; i++) {
            if (query_text[i] == '%') {
                key.push_back((char) (std::stoi(query_text.substr(i + 1, 2), 0, 16)));
                i += 2;
            } else {
                key.push_back(query_text[i]);
            }
        }
        size_t ampersand = query_text.find('&', equal_sign);
        size_t end = ampersand == std::string::npos ? query_text.size() : ampersand;
        for (size_t i = equal_sign + 1; i < end; i++) {
            if (query_text[i] == '%') {
                val.push_back((char) (std::stoi(query_text.substr(i + 1, 2), 0, 16)));
                i += 2;
            } else {
                val.push_back(query_text[i]);
            }
        }
        req.queries[key] = val;
        if (ampersand == std::string::npos)
            break;
        begin = ampersand + 1;
    }
}
