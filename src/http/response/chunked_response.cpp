#include <iostream>
#include "chunked_response.hpp"

ChunkedResponse::ChunkedResponse(HttpVersion version) : Response(version) {
    makeChunked();
}

ChunkedResponse::ChunkedResponse(const Response &resp) : Response(resp), finished(false) {
    if (!resp.body.empty())
        chunks.push(std::make_shared<Chunk>(resp.body));
    body.clear();
    makeChunked();
}

bool ChunkedResponse::empty() {
    return chunks.empty();
}

ChunkPtr ChunkedResponse::popChunk() {
    auto chunk = chunks.front();
    chunks.pop();
    return chunk;
}

void ChunkedResponse::pushChunk(ChunkPtr chunk) {
    chunks.push(chunk);
}

Chunk::Chunk(size_t len) : len(len) {
    buf = new char[len];
}

Chunk::~Chunk() {
    delete[] buf;
}

Chunk::Chunk(const std::string &data) : Chunk(data.length()) {
    memcpy(buf, data.data(), data.length());
}
