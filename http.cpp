#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <uv.h>
#include "middleware.h"
#include "http.h"

HttpServer::HttpServer(Middleware *middleware, const std::string bind_addr, int port, bool log) : middleware(
    middleware), bind_addr(bind_addr), port(port) {
    uv_loop_init(&log_init_loop);
    if (log) {
        set_info_log("info.log");
        set_error_log("error.log");
    }
    uv_tcp_init(uv_default_loop(), &server);
    uv_ip4_addr(bind_addr.c_str(), port, &addr);
    uv_tcp_bind(&server, (const sockaddr *) &addr, 0);
    server.data = this;
}

HttpServer::~HttpServer() {
    delete middleware;
}

static void __close_callback(uv_handle_t *handle) {
    delete (Request *) handle->data;
    delete (uv_tcp_t *) handle;
}

static void __alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

static void __read_callback(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    Request *req = (Request *) client->data;
    if (nread > 0) {
        req->push_buf(buf, nread);
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            req->error_log(std::string("Read error: ") + uv_strerror((int) nread) + "\n");
        uv_close((uv_handle_t *) client, __close_callback);
    }
    delete[] buf->base;
}

struct write_response_req {
    uv_write_t wt;
    HttpServer *server;
};

static void __write_callback(uv_write_t *req, int status) {
    HttpServer *server = ((write_response_req *) req)->server;
    if (status < 0) {
        server->error_log(std::string("Write error: ") + uv_strerror(status) + "\n");
        // error!
    }
    delete (std::vector<char> *) req->data;
    delete (write_response_req *) req;
}

static void __on_new_connection(uv_stream_t *tcp, int status) {
    HttpServer *server = (HttpServer *) tcp->data;
    if (status < 0) {
        server->error_log(std::string("New connection error: ") + uv_strerror(status));
        return;
    }
    uv_tcp_t *client = new uv_tcp_t;
    uv_tcp_init(uv_default_loop(), client);
    client->data = new Request(server, client);
    if (uv_accept(tcp, (uv_stream_t *) client) == 0) {
        uv_read_start((uv_stream_t *) client, __alloc_buffer, __read_callback);
    } else {
        uv_close((uv_handle_t *) client, __close_callback);
    }
}

void HttpServer::write_response(uv_stream_t *client, const Response &resp) {
    std::vector<char> *buf_vec = new std::vector<char>;
    std::stringstream status_line;
    status_line << resp.http_version << " " << (int) resp.status_code << " " << resp.reason_phrase << "\r\n";
    std::string str = status_line.str();
    buf_vec->insert(buf_vec->end(), str.begin(), str.end());
    for (auto &e : resp.headers) {
        std::stringstream header;
        header << e.first << ": " << e.second << "\r\n";
        str = header.str();
        buf_vec->insert(buf_vec->end(), str.begin(), str.end());
    }
    if (resp.headers.find("Content-Length") == resp.headers.end()) {
        std::stringstream header;
        header << "Content-Length: " << resp.body.size() << "\r\n";
        str = header.str();
        buf_vec->insert(buf_vec->end(), str.begin(), str.end());
    }
    buf_vec->insert(buf_vec->end(), {'\r', '\n'});
    buf_vec->insert(buf_vec->end(), resp.body.begin(), resp.body.end());
    delete &resp;
    write_response_req *req = new write_response_req;
    req->wt.data = buf_vec;
    req->server = this;
    uv_buf_t buf = uv_buf_init(buf_vec->data(), buf_vec->size());
    uv_write((uv_write_t *) req, client, &buf, 1, __write_callback);
}

void HttpServer::start() {
    std::stringstream ss;
    ss << "Start listening " << bind_addr << " port " << port << "\n";
    info_log(ss.str());
    int r = uv_listen((uv_stream_t *) &server, DEFAULT_BACKLOG, __on_new_connection);
    if (r) {
        fprintf(stderr, "Listen error %s\n", uv_strerror(r));
        exit(1);
    }
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

void HttpServer::process(const Request &req) {
    Response *resp = new Response(req.get_http_version());
    middleware->call(req, *resp);
    uv_tcp_t *client = req.client;
    delete &req;
    write_response((uv_stream_t *) client, *resp);
}

struct fs_write_req {
    uv_fs_t wt;
    uv_buf_t buf;
};

void HttpServer::set_info_log(const std::string &path) {
    info_log_path = path;
    info_log_fs.data = this;
    uv_fs_open(uv_default_loop(), &info_log_fs, path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644, NULL);
    info_log_enabled = true;
    info_log_fd = info_log_fs.result;
}

void HttpServer::set_error_log(const std::string &path) {
    this->error_log_path = path;
    error_log_fs.data = this;
    uv_fs_open(uv_default_loop(), &error_log_fs, path.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644, NULL);
    error_log_enabled = true;
    error_log_fd = error_log_fs.result;
}

static void __write_log_callback(uv_fs_t *req) {
    if (req->result < 0) {
        fprintf(stderr, "Write log error: %s\n", uv_strerror((int) req->result));
    }
    fs_write_req *write_req = (fs_write_req *) req;
    delete[] write_req->buf.base;
    delete req;
}

void HttpServer::info_log(const std::string &msg) {
    std::cout << msg;
    if (is_info_log_enabled()) {
        fs_write_req *write_req = new fs_write_req;
        write_req->buf = uv_buf_init(new char[msg.size()], msg.size());
        memcpy(write_req->buf.base, msg.data(), msg.size());
        uv_fs_write(uv_default_loop(), (uv_fs_t *) write_req, info_log_fd, &write_req->buf, 1, -1,
                    __write_log_callback);
    }
}

void HttpServer::error_log(const std::string &msg) {
    std::cerr << msg;
    if (is_error_log_enabled()) {
        fs_write_req *write_req = new fs_write_req;
        write_req->buf = uv_buf_init(new char[msg.size()], msg.size());
        memcpy(write_req->buf.base, msg.data(), msg.size());
        uv_fs_write(uv_default_loop(), (uv_fs_t *) write_req, error_log_fd, &write_req->buf, 1, -1,
                    __write_log_callback);
    }
}

bool HttpServer::is_info_log_enabled() const {
    return info_log_enabled;
}

bool HttpServer::is_error_log_enabled() const {
    return error_log_enabled;
}
