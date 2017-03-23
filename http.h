#ifndef RACKCPP_HTTP_H
#define RACKCPP_HTTP_H

#include <string>
#include <map>
#include <uv.h>
#include "request.h"
#include "response.h"

class Middleware;

class HttpServer {
    Middleware *middleware;
    std::string bind_addr;
    int port;
    uv_tcp_t server;
    sockaddr_in addr;
    bool info_log_enabled;
    std::string info_log_path;
    uv_fs_t info_log_fs;
    int info_log_fd;
    bool error_log_enabled;
    std::string error_log_path;
    uv_fs_t error_log_fs;
    int error_log_fd;
    uv_loop_t log_init_loop;

    const int DEFAULT_BACKLOG = 128;

    void write_response(uv_stream_t *client, const Response &resp);

    friend void __open_info_log_callback(uv_fs_t *req);

    friend void __open_error_log_callback(uv_fs_t *req);

public:
    HttpServer(Middleware *middleware, const std::string bind_addr, int port, bool logger);

    ~HttpServer();

    void set_info_log(const std::string &path);

    void set_error_log(const std::string &path);

    void info_log(const std::string &msg);

    void error_log(const std::string &msg);

    bool is_info_log_enabled() const;

    bool is_error_log_enabled() const;

    void start();

    void process(const Request &req);
};

#endif
