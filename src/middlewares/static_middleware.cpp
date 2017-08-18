#include "static_middleware.hpp"
#include "../utils/logger.hpp"

MiddlewarePtr StaticMiddleware::call(const Request &req, std::shared_ptr<Response> &resp) {
    if (!file.is_open()) {
        resp->setStatusCode(StatusCode::HTTP_NOT_FOUND);
        resp->body = "404 Not Found";
        return nullptr;
    }

    if (!resp->isChunked()) {
        resp.reset(new ChunkedResponse(*resp));
        return shared_from_this();
    }

    auto chunkedResp = std::dynamic_pointer_cast<ChunkedResponse>(resp);
    if (file.eof()) {
        chunkedResp->finished = true;
    } else {
        char buf[4096];
        file.read(buf, sizeof(buf));
        auto len = file.gcount();
        std::string data(buf, len);
        chunkedResp->pushChunk(data);
    }

    return shared_from_this();
}

StaticMiddleware::StaticMiddleware(const std::string &path)
        : path(path), file(path, std::ios::binary | std::ios::in) {
}