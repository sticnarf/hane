#include "static_middleware.hpp"
#include "../utils/logger.hpp"

MiddlewarePtr StaticMiddleware::call(RequestPtr req, std::shared_ptr<Response> &resp) {
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
        const size_t BUF_SIZE = 65536;

        auto chunk = std::make_shared<Chunk>(BUF_SIZE);
        file.read(chunk->buf, chunk->len);
        chunk->len = static_cast<size_t>(file.gcount());
        chunkedResp->pushChunk(chunk);
    }

    return shared_from_this();
}

StaticMiddleware::StaticMiddleware(const std::string &path)
        : path(path), file(path, std::ios::binary | std::ios::in) {
}