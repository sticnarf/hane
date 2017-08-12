#ifndef HANE_CHUNKED_RESPONSE_HPP
#define HANE_CHUNKED_RESPONSE_HPP

#include "response.hpp"
#include <queue>

/**
 * A ChunkedResponse must have header field "Transfer-Encoding: chunked",
 * and must have an empty body.
 */
class ChunkedResponse : public Response {
    std::queue<std::string> chunks;
public:
    bool finished;

    explicit ChunkedResponse(HttpVersion version);

    ChunkedResponse(const ChunkedResponse &) = default;

    ChunkedResponse(const Response &resp);

    void pushChunk(std::string &&chunk);

    std::string popChunk();

    bool empty();
};

#endif
