#include <string>
#include <cctype>
#include <uv.h>
#include "request.h"

Request::Request(HttpServer *http_server, uv_tcp_t *client) : http_server(http_server), client(client) {}

void Request::push_buf(const uv_buf_t *buf, ssize_t nread) {
    parser.push_buf(buf->base, nread);
    delete[] buf->base;
    parser.parse(*this);
}

void Request::process() {
    http_server->process(*this);
}

HttpServer::Method Request::get_method() const {
    return method;
}

const std::string &Request::get_request_target() const {
    return request_target;
}

const std::string &Request::get_http_version() const {
    return http_version;
}

const std::unordered_map<std::string, std::string> &Request::get_headers() const {
    return headers;
}

const std::vector<char> &Request::get_body() const {
    return body;
}

void Request::Parser::parse_request_line(Request &req) {
    size_t line_end = buf.find("\r\n", buf_pos);
    if (line_end != std::string::npos) {
        size_t sep = buf.find(' ', buf_pos);
        req.method = HttpServer::parse_method(buf.substr(buf_pos, (sep++) - buf_pos));
        size_t sep2 = buf.find(' ', sep);
        req.request_target = buf.substr(sep, (sep2++) - sep);
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
        req.headers.insert({field_name, field_value});
        buf_pos = line_end + 2;
        parse(req);
    }
}

void Request::Parser::parse_message_body(Request &req) {
    if (req.method == HttpServer::Method::HTTP_GET) {
        stage = Stage::PARSING_FINISHED;
        parse(req);
        return;
    }
    auto transfer_encoding = req.headers.find("Transfer-Encoding");
    if (transfer_encoding != req.headers.end()) {
        // TODO: Transfer-Encoding is not supported
    } else {
        int content_length = stoi(req.headers["Content-Length"]);
        if (buf.size() - buf_pos >= content_length) {
            auto body_start = buf.begin() + buf_pos;
            auto body_end = body_start + content_length;
            req.body.insert(req.body.end(), body_start, body_end);
            auto new_req = new Request(req.http_server, req.client);
            auto buf_left = buf.substr(buf_pos + content_length);
            new_req->parser.push_buf(buf_left.c_str(), buf_left.size());
            req.client->data = new_req;
            stage = Stage::PARSING_FINISHED;
            parse(req);
        }
    }
    // End of Parsing
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
        case Stage::PARSING_FINISHED:
            req.process();
            break;
    }
}