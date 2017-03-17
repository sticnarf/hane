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
    uv_ip4_addr(bind_addr.c_str(), port, &addr);
    uv_tcp_bind(&server, (const sockaddr *) &addr, 0);
    server.data = this;
}

HttpServer::~HttpServer() {
    delete middleware;
}

void alloc_buffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void __read_callback(uv_stream_t *client, ssize_t nread, const uv_buf_t *buf) {
    if (nread > 0) {
        Request *req = (Request *) client->data;
        req->push_buf(buf, nread);
        return;
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            fprintf(stderr, "Read error %s\n", uv_err_name((int) nread));
        uv_close((uv_handle_t *) client, NULL);
        delete client;
    }

    delete[] buf->base;
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
    if (uv_accept(tcp, (uv_stream_t *) client) == 0) {
        uv_read_start((uv_stream_t *) client, alloc_buffer, __read_callback);
    } else {
        uv_close((uv_handle_t *) client, NULL);
        delete client;
    }
}

void HttpServer::write_response(uv_stream_t *client, const Response &resp) {
    std::vector<uv_buf_t> bufs;
    std::stringstream status_line;
    status_line << resp.http_version << " " << (int) resp.status_code << " " << resp.reason_phrase << "\r\n";
    bufs.push_back(uv_buf_init(new char[status_line.str().length()], status_line.str().length()));
    memcpy(bufs.back().base, status_line.str().c_str(), status_line.str().length());
    for (auto &e : resp.headers) {
        std::stringstream header;
        header << e.first << ": " << e.second << "\r\n";
        bufs.push_back(uv_buf_init(new char[header.str().length()], header.str().length()));
        memcpy(bufs.back().base, header.str().c_str(), header.str().length());
    }
    if (resp.headers.find("Content-Length") == resp.headers.end()) {
        std::stringstream header;
        header << "Content-Length: " << resp.body.size() << "\r\n";
        bufs.push_back(uv_buf_init(new char[header.str().length()], header.str().length()));
        memcpy(bufs.back().base, header.str().c_str(), header.str().length());
    }
    bufs.push_back(uv_buf_init(new char[2], 2));
    memcpy(bufs.back().base, "\r\n", 2);
    bufs.push_back(uv_buf_init(new char[resp.body.size()], resp.body.size()));
    memcpy(bufs.back().base, &resp.body[0], resp.body.size());
    //delete &resp;
    uv_write(new uv_write_t, client, &bufs[0], bufs.size(), NULL);
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
    //delete &req;
    write_response((uv_stream_t *) req.client, *resp);
}
