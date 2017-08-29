#ifndef HANE_CLIENT_HPP
#define HANE_CLIENT_HPP

#include "./parser/parser.hpp"
#include "./http.hpp"
#include <mutex>
#include <condition_variable>

class Client {
private:
    HttpServer *server;
//    uv_idle_t idler;
    uv_tcp_t *tcp;
    uv_write_t write;
    uv_buf_t buf;
    Parser parser;
    std::mutex modifyQueuedMutex;
    std::mutex queueMutex;
    std::mutex awaitMutex;
    std::mutex closeMutex;
    std::mutex processMutex;
    std::condition_variable closeCv;
    std::condition_variable awaitCv;
    bool closed;

    RequestPtr currRequest;
    std::shared_ptr<ChunkedResponse> currResponse;
    MiddlewarePtr currMiddleware;
    int queued;
    std::condition_variable queueCv;

    static void pushBuffer(uv_work_t *work);

    static void pushBufferCallback(uv_work_t *work, int status);

public:
    explicit Client(HttpServer *server);

    void pushBuf(const char *buf, size_t len);

    static void startProcessing(uv_work_t *work);

    static void startProcessingCallback(uv_work_t *work, int status);

    void processRequest(std::unique_lock<std::mutex> *processLock);

    void closeConnection();

    ~Client();

    friend class HttpServer;
    friend class Parser;
};

#endif
