#include <iostream>
#include "client.hpp"
#include "errors.hpp"
#include "../utils/logger.hpp"

Client::Client(HttpServer *server)
        : server(server), parser(this), queued(0) {
    tcp = new uv_tcp_t;
    tcp->data = this;
}

struct PushBufferHandler {
    Client *client;
    const char *buf;
    size_t len;

    PushBufferHandler(Client *client, const char *buf, size_t len) : client(client), buf(buf), len(len) {}
};

void Client::pushBuffer(uv_work_t *work) {
    auto handler = static_cast<PushBufferHandler *>(work->data);
    auto client = handler->client;
    client->parser.pushBuf(handler->buf, handler->len);
}

void Client::pushBufferCallback(uv_work_t *work, int status) {
    if (status < 0)
        Logger::getInstance().error("PushBuffer error: {}", uv_strerror(status));

    auto handler = static_cast<PushBufferHandler *>(work->data);
    delete[] handler->buf;
    delete handler;
}

void Client::pushBuf(const char *buf, size_t len) {
    parser.pushBuf(buf, len);
//    auto *pushWork = new uv_work_t;
//    pushWork->data = new PushBufferHandler(this, buf, len);
//    uv_queue_work(uv_default_loop(), pushWork, pushBuffer, pushBufferCallback);
}

Client::~Client() {
    delete tcp;
}

static std::shared_ptr<Response> buildErrorResponse(const HttpError &e) {
    // TODO Now use HTTP/1.1 arbitrarily
    auto resp = std::make_shared<Response>(HttpVersion::HTTP_1_1);

    resp->setStatusCode(e.getCode());
    resp->body = e.getReason();

    return resp;
}

void Client::processRequest(std::unique_lock<std::mutex> *processLock) {
    if (currRequest && currResponse && currMiddleware) {
        std::unique_lock<std::mutex> queueLock(queueMutex);
//        std::cout << "queueMutex locked in processRequest, start wait" << std::endl;
        queueCv.wait(queueLock, [&] {
            return queued < 8;
        });
//        std::cout << "queueMutex unlocked in processRequest, stop wait" << std::endl;
        auto polyResponse = std::dynamic_pointer_cast<Response>(currResponse);
        currMiddleware = currMiddleware->call(currRequest, polyResponse);
        server->writeChunks(AsyncChunkedResponseHandler(currRequest, currResponse),
                            reinterpret_cast<uv_stream_t *>(tcp),
                            processLock);

        if (currResponse->finished) {
            currMiddleware = nullptr;
            currRequest = nullptr;
            currResponse = nullptr;
            return;
        }
    }

    if (!parser.hasCompleteRequest())
        return;

    RequestPtr req = parser.yieldRequest();
    if (req->isBad()) {
        auto badRequest = std::dynamic_pointer_cast<BadRequest>(req);
        auto e = badRequest->getError();
        Logger::getInstance().error("Error code {}: {}", static_cast<int>(e.getCode()), e.getReason());
        auto errorResp = buildErrorResponse(e);
        server->writeResponse(reinterpret_cast<uv_stream_t *>(tcp), errorResp, processLock);
        return;
    }

    try {
        auto resp = std::make_shared<Response>(req->getHttpVersion());
        currMiddleware = server->middleware->call(req, resp);
        server->writeResponse(reinterpret_cast<uv_stream_t *>(tcp), resp, processLock);

        if (resp->isChunked()) {
            currRequest = req;
            currResponse = std::dynamic_pointer_cast<ChunkedResponse>(resp);
        }
    } catch (HttpError &e) {
        auto errorResp = buildErrorResponse(e);
        server->writeResponse(reinterpret_cast<uv_stream_t *>(tcp), errorResp, processLock);
    }
}

void Client::startProcessing(uv_work_t *work) {
    auto client = static_cast<Client *>(work->data);
    while (!client->closed) {
        std::unique_lock<std::mutex> awaitLock(client->awaitMutex);
        client->awaitCv.wait(awaitLock,
                             [&] {
                                 return (client->currRequest && client->currResponse &&
                                         client->currMiddleware) ||
                                        client->parser.hasCompleteRequest() || client->closed;
                             });


//        std::cout << "to lock process in processing" << std::endl;
        auto processLock = new std::unique_lock<std::mutex>(client->processMutex);
//        std::cout << "locked process in processing" << std::endl;
        if (client->closed) {
            processLock->unlock();
            delete processLock;
//            std::cout << "unlocked process in processing" << std::endl;
            break;
        }

        client->processRequest(processLock);
//        std::cout << "unlocked process in processing" << std::endl;
    }
}

void Client::startProcessingCallback(uv_work_t *work, int status) {
    if (status < 0)
        Logger::getInstance().error("StartProcessing error: {}", uv_strerror(status));

    delete static_cast<Client *>(work->data);
    delete work;
}

void Parser::process() {
    try {
        currentParser = currentParser->process();
    } catch (HttpError &e) {
        auto badRequest = std::make_shared<BadRequest>(e);
        completeRequests.push(badRequest);
        client->awaitCv.notify_one();
        buffer = std::make_shared<Buffer>();
        currentParser = std::make_shared<StartLineParser>(std::make_shared<Request>(), buffer);
    }

    if (currentParser->isFinished()) {
        completeRequests.push(currentParser->getRequest());
        client->awaitCv.notify_one();
        currentParser = std::make_shared<StartLineParser>(std::make_shared<Request>(), buffer);
    }
}

void Client::closeConnection() {
    {
        std::unique_lock<std::mutex> processLock(processMutex, std::defer_lock);
        std::unique_lock<std::mutex> closeLock(closeMutex, std::defer_lock);
//        std::cout << "to lock process in close" << std::endl;
        std::lock(processLock, closeLock);
//        std::cout << "locked process in close, start wait" << std::endl;
        closeCv.wait(closeLock, [&] {
            std::lock_guard<std::mutex> lk(modifyQueuedMutex);
//            std::cout << "notified: " << queued << std::endl;
            return queued == 0;
        });
//        std::cout << "stop wait" << std::endl;
        closed = true;
    }
    awaitCv.notify_all();

    uv_close(reinterpret_cast<uv_handle_t *>(this->tcp), nullptr);
}