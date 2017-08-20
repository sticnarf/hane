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
}

HttpServer::~HttpServer() = default;

static void closeCallback(uv_handle_t *handle) {
    delete static_cast<Client *>(handle->data);
}

static void allocBuffer(uv_handle_t *handle, size_t suggested_size, uv_buf_t *buf) {
    buf->base = new char[suggested_size];
    buf->len = suggested_size;
}

static std::shared_ptr<Response> buildErrorResponse(const HttpError &e) {
    // TODO Now use HTTP/1.1 arbitrarily
    auto resp = std::make_shared<Response>(HttpVersion::HTTP_1_1);

    resp->setStatusCode(e.getCode());
    resp->body = e.getReason();

    return resp;
}

struct WriteHandler {
    char *arr;
    Client *client;
    void *addition;

    WriteHandler(const WriteHandler &) = default;

    WriteHandler(char *arr, Client *client, void *addition) : arr(arr), client(client), addition(addition) {}
};

void readCallback(uv_stream_t *clientTcp, ssize_t nread, const uv_buf_t *buf) {
    auto *client = static_cast<Client *>(clientTcp->data);
    if (nread > 0) {
        Logger::getInstance().info("Read {} bytes.", nread);
        try {
            client->pushBuf(buf->base, nread);
        } catch (const HttpError &e) {
            Logger::getInstance().error("Error code {}: {}", static_cast<int>(e.getCode()), e.getReason());
            auto errorResp = buildErrorResponse(e);
            client->server->writeResponse(clientTcp, errorResp);
            client->closeConnection();
        }

    }
    if (nread < 0) {
        if (nread != UV_EOF)
            Logger::getInstance().error("Read error: {}", uv_strerror((int) nread));
        uv_close(reinterpret_cast<uv_handle_t *>(clientTcp), closeCallback);
    }
    delete[] buf->base;
}

void HttpServer::writeCallback(uv_write_t *req, int status) {
    if (status < 0) {
        Logger::getInstance().error("Write error: {}", uv_strerror(status));
        // error!
    }

    auto handler = static_cast<WriteHandler *>(req->data);
    handler->client->queued--;
    delete[] (handler->arr);
    delete handler;
    delete req;
}

void onNewConnection(uv_stream_t *serverTcp, int status) {
    auto *server = static_cast<HttpServer *>(serverTcp->data);
    if (status < 0) {
        Logger::getInstance().error("New connection error: {}", uv_strerror(status));
        return;
    }

    auto *client = new Client(server);

    uv_tcp_init(uv_default_loop(), client->tcp);
    auto uvTcp = reinterpret_cast<uv_stream_t *>(client->tcp);

    if (uv_accept(serverTcp, uvTcp) == 0) {
        uv_read_start(uvTcp, allocBuffer, readCallback);
    } else {
        uv_close(reinterpret_cast<uv_handle_t *>(client->tcp), closeCallback);
    }
}

void Client::closeConnection() {
    uv_close(reinterpret_cast<uv_handle_t *>(this->tcp), closeCallback);
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

void HttpServer::writeChunks(AsyncChunkedResponseHandler *handler, uv_stream_t *tcp) {
    std::string data;
    auto resp = handler->resp;
    if (resp->finished)
        data = "0\r\n\r\n";

    while (!resp->empty()) {
        auto chunk = resp->popChunk();
        data += fmt::format("{0:x}\r\n{1}\r\n", chunk.length(), chunk);
    }

    writeData(tcp, data, handler, writeChunkCallback);
}

void HttpServer::start() {
    Logger::getInstance().info("Start listening {} port {}", bindAddr, port);
    int r = uv_listen((uv_stream_t *) &server, DEFAULT_BACKLOG, onNewConnection);
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
 * @param client
 */
void HttpServer::process(const Request &req, uv_tcp_t *client) {
    auto resp = std::make_shared<Response>(req.getHttpVersion());
    auto currMiddleware = middleware->call(req, resp);
    writeResponse(reinterpret_cast<uv_stream_t *>(client), resp);

    if (resp->isChunked()) {
        auto chunkedResp = std::dynamic_pointer_cast<ChunkedResponse>(resp);
        processChunks(
                new AsyncChunkedResponseHandler(currMiddleware, req, chunkedResp),
                reinterpret_cast<uv_stream_t *>(client));
    }

    auto connectionEntry = req.getHeader().get("Connection");
    if (connectionEntry.isValid() && connectionEntry.getValue()->getContent() == "close")
        static_cast<Client *>(client->data)->closeConnection();
}

void HttpServer::writeData(uv_stream_t *tcp, const std::string &data, void *addition, uv_write_cb callback) {
    auto client = static_cast<Client *>(tcp->data);
    client->queued++;
    auto arr = new char[data.length()];
    memcpy(arr, data.data(), data.length());

//    auto *client = static_cast<Client *>(tcp->data);
//    client->write.data = arr;
//    client->buf = uv_buf_init(arr, static_cast<unsigned int>(data.length()));
    auto write = new uv_write_t;
    write->data = new WriteHandler(arr, client, addition);
    auto buf = uv_buf_init(arr, static_cast<unsigned int>(data.length()));

    uv_write(write, tcp, &buf, 1, callback);
}

void HttpServer::processChunks(AsyncChunkedResponseHandler *handler, uv_stream_t *tcp) {
//    auto client = static_cast<Client *>(tcp->data);
//    while (!handler->resp->finished && client->queued < 8) {
//        auto polyResp = std::dynamic_pointer_cast<Response>(handler->resp);
//        handler->currMiddleware = handler->currMiddleware->call(handler->req, polyResp);
//        writeChunks(handler, tcp);
//    }
    if (!handler->resp->finished) {
        auto polyResp = std::dynamic_pointer_cast<Response>(handler->resp);
        handler->currMiddleware = handler->currMiddleware->call(handler->req, polyResp);
        writeChunks(handler, tcp);
    } else
        delete handler;
}

void HttpServer::writeChunkCallback(uv_write_t *req, int status) {
    auto handler = static_cast<WriteHandler *>(req->data);
    auto asyncHandler = static_cast<AsyncChunkedResponseHandler *>(handler->addition);
    auto server = handler->client->server;
    auto tcp = handler->client->tcp;

    handler->client->queued--;

    auto currMiddleware = asyncHandler->currMiddleware;
    auto &chunkedReq = asyncHandler->req;
    auto resp = asyncHandler->resp;

    delete asyncHandler;
    delete[] (handler->arr);
    delete handler;
    delete req;

    if (status < 0) {
        Logger::getInstance().error("Write error: {}", uv_strerror(status));
        // error!
        return;
    }

    auto newAsyncHandler = new AsyncChunkedResponseHandler(currMiddleware, chunkedReq, resp);

    server->processChunks(newAsyncHandler, reinterpret_cast<uv_stream_t *>(tcp));

}
