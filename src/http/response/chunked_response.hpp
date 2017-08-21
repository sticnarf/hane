#ifndef HANE_CHUNKED_RESPONSE_HPP
#define HANE_CHUNKED_RESPONSE_HPP

#include "response.hpp"
#include <queue>
#include <memory>

struct Chunk {
    char* buf;
    size_t len;

    explicit Chunk(size_t len);
    Chunk(const std::string& data);
    ~Chunk();
};

typedef std::shared_ptr<Chunk> ChunkPtr;

/**
 * A ChunkedResponse must have header field "Transfer-Encoding: chunked",
 * and must have an empty body.
 */
class ChunkedResponse : public Response {
    std::queue<ChunkPtr> chunks;
public:
    bool finished;

    explicit ChunkedResponse(HttpVersion version);

    ChunkedResponse(const ChunkedResponse &) = default;

    ChunkedResponse(const Response &resp);

    void pushChunk(ChunkPtr chunk);

    ChunkPtr popChunk();

    bool empty();
};

#endif
