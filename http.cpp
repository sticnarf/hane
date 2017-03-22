#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <uv.h>
#include "middleware.h"
#include "http.h"
#include "request.h"

using namespace middleware;

HttpServer::HttpServer(Middleware *middleware, const std::string bind_addr, int port) : middleware(middleware),
                                                                                        bind_addr(bind_addr),
                                                                                        port(port) {
    uv_tcp_init(uv_default_loop(), &server);
    uv_tcp_keepalive(&server, 1, 60);
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
    fprintf(stdout, "Clear client: %p\n", handle);
}


static void __alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

static void __read_callback(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        Request *req = (Request *) client->data;
        fprintf(stdout, "Read %ld bytes! Request: %p\n", nread, req);
        req->push_buf(buf, nread);
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name((int) nread));
        uv_close((uv_handle_t *) client, __close_callback);
    }
    delete[] buf->base;
}

static void __write_callback(uv_write_t *req, int status) {
    if (status < 0) {
        fprintf(stderr, "Write error %s\n", uv_strerror(status));
        // error!
    }
    fprintf(stdout, "Write to client: %p\n", req->handle);
    delete (std::vector<char> *) req->data;
    delete req;
}

static void __on_new_connection(uv_stream_t *tcp, int status) {
    if (status < 0) {
        fprintf(stderr, "New connection error %s\n", uv_strerror(status));
        // error!
        return;
    }
    uv_tcp_t *client = new uv_tcp_t;
    uv_tcp_init(uv_default_loop(), client);
    client->data = new Request((HttpServer *) tcp->data, client);
    fprintf(stdout, "New connection! Client: %p\n", client);
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
    uv_write_t *wt = new uv_write_t;
    wt->data = buf_vec;
    uv_buf_t buf = uv_buf_init(buf_vec->data(), buf_vec->size());
    uv_write(wt, client, &buf, 1, __write_callback);
}

void HttpServer::start() {
    std::cout << bind_addr << std::endl;
    std::cout << port << std::endl;
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
