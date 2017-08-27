#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <utility>
#include <uv.h>
#include "response/chunked_response.hpp"
#include "../utils/logger.hpp"
#include "../utils/protocol_helper.hpp"
#include "client.hpp"
#include "errors.hpp"

HttpServer::HttpServer(std::shared_ptr<Middleware> middleware, const std::string &_bindAddr, int port)
        : middleware(std::move(middleware)), bindAddr(_bindAddr), port(port) {
    uv_tcp_init(uv_default_loop(), &server);
    uv_ip4_addr(_bindAddr.c_str(), port, &addr);
    uv_tcp_bind(&server, reinterpret_cast<const sockaddr *>(&addr), 0);
    server.data = this;
    uv_async_init(uv_default_loop(), &async, realWriteData);
}

HttpServer::~HttpServer() = default;

//void HttpServer::closeCallback(uv_handle_t *handle) {
////    auto client = static_cast<Client *>(handle->data);
////    client->closed = true;
////    client->awaitCv.notify_all();
//}

static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

void HttpServer::readCallback(uv_stream_t *clientTcp, ssize_t nread, const uv_buf_t *buf) {
    auto *client = static_cast<Client *>(clientTcp->data);
    if (nread > 0) {  // Read successfully
        client->pushBuf(buf->base, nread);
    }
    if (nread < 0) {
        if (nread != UV_EOF)
            Logger::getInstance().error("Read error: {}", uv_strerror((int) nread));

        client->closeConnection();
//        uv_close(reinterpret_cast<uv_handle_t *>(clientTcp), closeCallback);
    }
    delete[] buf->base;
}

void HttpServer::onNewConnection(uv_stream_t *serverTcp, int status) {
    if (status < 0) {
        Logger::getInstance().error("New connection error: {}", uv_strerror(status));
        return;
    }

    auto *server = static_cast<HttpServer *>(serverTcp->data);
    auto *client = new Client(server);
    client->closed = false;

    auto work = new uv_work_t;
    work->data = client;
    uv_queue_work(uv_default_loop(), work, Client::startProcessing, Client::startProcessingCallback);

    uv_tcp_init(uv_default_loop(), client->tcp);
    auto uvTcp = reinterpret_cast<uv_stream_t *>(client->tcp);

    if (uv_accept(serverTcp, uvTcp) == 0) {
        // Success. Start reading from the connection:
        uv_read_start(uvTcp, allocBuffer, readCallback);
    } else {
        // Fail. Close the connection directly:
//        uv_close(reinterpret_cast<uv_handle_t *>(client->tcp), closeCallback);
        client->closeConnection();
    }
}

void HttpServer::writeResponse(uv_stream_t *tcp, std::shared_ptr<const Response> resp) {
    std::stringstream responseText;

    // Append status line
    responseText << stringify(resp->httpVersion) << " "
                 << (int) resp->statusCode << " " << toReasonPhrase(resp->statusCode) << "\r\n";

    // Append headers
    for (auto &e : resp->headers.innerMap) {
        auto pair = e.second;
        responseText << pair.getKey() << ": " << pair.getValue()->getContent() << "\r\n";
    }

    // Append cookies
    for (auto &e : resp->cookies) {
        auto cookie = e.second;
        responseText << "Set-Cookie: " << cookie.toCookieString() << "\r\n";
    }

    // Append Content-Length if not chunked
    auto contentLengthEntry = resp->headers.get("Content-Length");
    if (!resp->isChunked() && !contentLengthEntry.isValid()) {
        responseText << "Content-Length: " << resp->body.size() << "\r\n";
    }

    // Append Body
    responseText << "\r\n" << resp->body;

    std::string str = responseText.str();

    writeData(tcp, str);
}

void HttpServer::writeChunks(AsyncChunkedResponseHandler handler, uv_stream_t *tcp) {
    std::string data;
    auto resp = handler.resp;
    if (resp->finished)
        data = "0\r\n\r\n";

    while (!resp->empty()) {
        auto chunk = resp->popChunk();
        data += fmt::format("{0:x}\r\n", chunk->len);
        data.insert(data.end(), chunk->buf, chunk->buf + chunk->len);
        data += "\r\n";
    }

    writeData(tcp, data, new AsyncChunkedResponseHandler(handler), writeChunkCallback);
}

void HttpServer::start() {
    Logger::getInstance().info("Start listening {} port {}", bindAddr, port);
    int r = uv_listen(reinterpret_cast<uv_stream_t *>(&server), DEFAULT_BACKLOG, onNewConnection);
    if (r > 0) {
        Logger::getInstance().error("Listen error {}", uv_strerror(r));
        exit(1);
    }
    uv_run(uv_default_loop(), UV_RUN_DEFAULT);
}

/**
 * This function will call the middleware.
 * After calling the middleware, if the response has "Transfer-Encoding: chunked",
 * the response should be able to and will be cast to a ChunkedResponse.
 * Then the middleware returned back will be repeatedly called until the response is finished.
 * @param req
 * @param tcp
 */
void HttpServer::process(RequestPtr req, uv_tcp_t *tcp) {
    auto resp = std::make_shared<Response>(req->getHttpVersion());
    auto client = static_cast<Client *>(tcp->data);
    client->currMiddleware = middleware->call(req, resp);
    writeResponse(reinterpret_cast<uv_stream_t *>(tcp), resp);

    if (resp->isChunked()) {
        auto chunkedResp = std::dynamic_pointer_cast<ChunkedResponse>(resp);
        processChunks(
                AsyncChunkedResponseHandler(req, chunkedResp),
                reinterpret_cast<uv_stream_t *>(tcp));
    }
}

struct AsyncSendHandler {
    uv_write_t *write;
    uv_stream_t *tcp;
    uv_buf_t buf;
    uv_write_cb callback;
    std::unique_lock<std::mutex> *lock;

    AsyncSendHandler(uv_write_t *write,
                     uv_stream_t *tcp,
                     const uv_buf_t &buf,
                     uv_write_cb callback,
                     std::unique_lock<std::mutex> *lock)
            : write(write), tcp(tcp), buf(buf), callback(callback), lock(lock) {}
};

struct WriteHandler {
    char *arr;
    Client *client;
    AsyncSendHandler *asyncSendHandler;
    void *addition;

    WriteHandler(const WriteHandler &) = default;

    WriteHandler(char *arr, Client *client, AsyncSendHandler *asyncHandler, void *addition)
            : arr(arr), client(client), asyncSendHandler(asyncHandler), addition(addition) {}
};

void HttpServer::writeData(uv_stream_t *tcp, const std::string &data, void *addition, uv_write_cb callback) {
    auto lock = new std::unique_lock<std::mutex>(writeMutex);
    auto client = static_cast<Client *>(tcp->data);
    client->queued++;
    auto arr = new char[data.length()];
    memcpy(arr, data.data(), data.length());

    auto write = new uv_write_t;
    auto buf = uv_buf_init(arr, static_cast<unsigned int>(data.length()));
    auto asyncSendData = new AsyncSendHandler(write, tcp, buf, callback, lock);
    write->data = new WriteHandler(arr, client, asyncSendData, addition);

    async.data = asyncSendData;
    uv_async_send(&async);
}

void HttpServer::processChunks(AsyncChunkedResponseHandler handler, uv_stream_t *tcp) {
    auto client = static_cast<Client *>(tcp->data);
    if (handler.resp->finished) {
        // Process the next request
        client->processRequest();
        return;
    }

    while (!handler.resp->finished && client->queued < 8) {
        auto polyResp = std::dynamic_pointer_cast<Response>(handler.resp);
        client->currMiddleware = client->currMiddleware->call(handler.req, polyResp);
        writeChunks(handler, tcp);
    }
}

void HttpServer::writeChunkCallback(uv_write_t *req, int status) {
    auto handler = static_cast<WriteHandler *>(req->data);
    auto asyncHandler = static_cast<AsyncChunkedResponseHandler *>(handler->addition);

    handler->client->queued--;
    handler->client->queueCv.notify_one();

    delete asyncHandler;
    delete[] (handler->arr);
    delete handler->asyncSendHandler;
    delete handler;
    delete req;

    if (status < 0) {
        Logger::getInstance().error("Write error: {}", uv_strerror(status));
        // error!
//        return;
    }
}

void HttpServer::realWriteData(uv_async_t *handle) {
    auto asyncHandler = static_cast<AsyncSendHandler *>(handle->data);
    uv_write(asyncHandler->write, asyncHandler->tcp, &asyncHandler->buf, 1, asyncHandler->callback);
    asyncHandler->lock->unlock();
    delete asyncHandler->lock;
}

void HttpServer::writeCallback(uv_write_t *req, int status) {
    if (status < 0) {
        Logger::getInstance().error("Write error: {}", uv_strerror(status));
        // error!
    }

    auto handler = static_cast<WriteHandler *>(req->data);
    handler->client->queued--;
    delete[] (handler->arr);
    delete handler->asyncSendHandler;
    delete handler;
    delete req;
}