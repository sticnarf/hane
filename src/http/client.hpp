#ifndef HANE_CLIENT_HPP
#define HANE_CLIENT_HPP

#include "./parser/parser.hpp"
#include "./http.hpp"
#include <mutex>
#include <condition_variable>

class Client {
private:
    HttpServer *server;
    uv_tcp_t *tcp;
    uv_buf_t buf;
    Parser parser;
    std::mutex awaitMutex;
    std::condition_variable awaitCv;
    std::atomic_bool closed;

    RequestPtr currRequest;
    std::shared_ptr<ChunkedResponse> currResponse;
    MiddlewarePtr currMiddleware;
    std::atomic_int queued;
    std::condition_variable queueCv;

    static void pushBuffer(uv_work_t *work);

    static void pushBufferCallback(uv_work_t *work, int status);

    static void realCloseConnection(uv_work_t *work);

    static void realCloseConnectionCallback(uv_work_t *work, int status);

    static void closeCallback(uv_handle_t* handle);

public:
    explicit Client(HttpServer *server);

    void pushBuf(const char *buf, size_t len);

    static void startProcessing(uv_work_t *work);

    static void startProcessingCallback(uv_work_t *work, int status);

    void processRequest();

    void closeConnection();

    ~Client();

    friend class HttpServer;
    friend class Parser;
};

#endif
