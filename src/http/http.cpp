#include <string>
#include <iostream>
#include <sstream>
#include <cstring>
#include <utility>
#include <uv.h>
#include "../utils/logger.hpp"
#include "../utils/protocol_helper.hpp"
#include "client.hpp"
#include "../middlewares/middleware.hpp"
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

static void writeCallback(uv_write_t *req, int status) {
    if (status < 0) {
        Logger::getInstance().error("Write error: {}", uv_strerror(status));
        // error!
    }

    delete[] static_cast<char *>(req->data);
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
    for (auto &e : resp->headers) {
        responseText << e.first << ": " << e.second << "\r\n";
    }

    // Append Content-Length
    if (resp->headers.find("Content-Length") == resp->headers.end()) {
        responseText << "Content-Length: " << resp->body.size() << "\r\n";
    }

    // Append Body
    responseText << "\r\n" << resp->body;

    std::string str = responseText.str();

    auto data = new char[str.length()];
    memcpy(data, str.data(), str.length());

    auto *client = static_cast<Client *>(tcp->data);
    client->write.data = data;

    client->buf = uv_buf_init(data, static_cast<unsigned int>(str.length()));
    uv_write(&(client->write), tcp, &(client->buf), 1, writeCallback);
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

void HttpServer::process(const Request &req, uv_tcp_t *client) {
    auto resp = std::make_shared<Response>(req.getHttpVersion());
    middleware->call(req, resp);
    writeResponse(reinterpret_cast<uv_stream_t *>(client), resp);

    auto connectionEntry = req.getHeader().get("Connection");
    if (connectionEntry.isValid() && connectionEntry.getValue()->getContent() == "close")
        static_cast<Client *>(client->data)->closeConnection();
}
