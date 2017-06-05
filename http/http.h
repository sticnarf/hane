#ifndef RACKCPP_HTTP_H
#define RACKCPP_HTTP_H

#include <string>
#include <map>
#include <uv.h>
#include "request.h"
#include "response.h"

class Middleware;

class HttpServer
{
    Middleware* middleware;
    std::string bindAddr;
    int port;
    uv_tcp_t server;
    sockaddr_in addr;
    bool infoLogEnabled;
    std::string infoLogPath;
    uv_fs_t infoLogFs;
    int infoLogFd;
    bool errorLogEnabled;
    std::string errorLogPath;
    uv_fs_t errorLogFs;
    int errorLogFd;
    uv_loop_t logInitLoop;

    const int DEFAULT_BACKLOG = 128;

    void writeResponse(uv_stream_t* client, const Response& resp);

    friend void __open_info_log_callback(uv_fs_t* req);

    friend void __open_error_log_callback(uv_fs_t* req);

public:
    HttpServer(Middleware* middleware, const std::string _bindAddr, int port, bool logger);

    ~HttpServer();

    void setInfoLog(const std::string& path);

    void setErrorLog(const std::string& path);

    void infoLog(const std::string& msg);

    void errorLog(const std::string& msg);

    bool isInfoLogEnabled() const;

    bool isErrorLogEnabled() const;

    void start();

    void process(const Request& req);
};

#endif
