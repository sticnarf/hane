#ifndef HANE_CHUNKED_RESPONSE_HPP
#define HANE_CHUNKED_RESPONSE_HPP

#include "response.hpp"
#include <queue>

class ChunkedResponse : Response {
    std::queue<std::string> chunks;
public:
    explicit ChunkedResponse(HttpVersion version);

    ChunkedResponse(const ChunkedResponse &) = default;

    ChunkedResponse(const Response &resp);

    void pushChunk(std::string &&chunk);

    std::string popChunk();

    bool empty();
};

#endif
