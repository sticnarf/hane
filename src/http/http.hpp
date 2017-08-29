#ifndef HANE_HTTP_HPP
#define HANE_HTTP_HPP

#include <string>
#include <map>
#include <memory>
#include <utility>
#include <uv.h>
#include <mutex>
#include <atomic>
#include "../middlewares/middleware.hpp"
#include "./request/request.hpp"
#include "./response/response.hpp"
#include "./response/chunked_response.hpp"

struct AsyncChunkedResponseHandler {
    RequestPtr req;
    std::shared_ptr<ChunkedResponse> resp;

    AsyncChunkedResponseHandler(RequestPtr req,
                                std::shared_ptr<ChunkedResponse> resp)
            : req(req), resp(resp) {}
};

class HttpServer {
    std::shared_ptr<Middleware> middleware;
    std::string bindAddr;
    int port;
    uv_tcp_t server;
    sockaddr_in addr;
    std::mutex writeMutex;
    uv_async_t writeAsync;
    std::mutex closeMutex;
    uv_async_t closeAsync;

    const int DEFAULT_BACKLOG = 128;

    static void writeCallback(uv_write_t *req, int status);

    static void writeChunkCallback(uv_write_t *req, int status);

    static void onNewConnection(uv_stream_t *serverTcp, int status);

    static void readCallback(uv_stream_t *clientTcp, ssize_t nread, const uv_buf_t *buf);

    static void realWriteData(uv_async_t *handle);

    static void realCloseConnection(uv_async_t *handle);

    void writeData(uv_stream_t *client, const std::string &data,
                   void *addition, uv_write_cb callback);

    void processChunks(AsyncChunkedResponseHandler handler, uv_stream_t *client);

public:
    HttpServer(std::shared_ptr<Middleware> middleware, const std::string &_bindAddr, int port);

    ~HttpServer();

    void start();

    void process(RequestPtr req, uv_tcp_t *tcp);

    void writeResponse(uv_stream_t *client, std::shared_ptr<const Response> resp);

    void writeChunks(AsyncChunkedResponseHandler handler, uv_stream_t *tcp);

    friend class Client;
};

#endif
