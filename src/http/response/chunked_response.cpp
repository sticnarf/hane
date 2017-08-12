#include <iostream>
#include "chunked_response.hpp"

ChunkedResponse::ChunkedResponse(HttpVersion version) : Response(version) {
    makeChunked();
}

ChunkedResponse::ChunkedResponse(const Response &resp) : Response(resp), finished(false) {
    if(!resp.body.empty())
        chunks.push(resp.body);
    body.clear();
    makeChunked();
}

bool ChunkedResponse::empty() {
    return chunks.empty();
}

std::string ChunkedResponse::popChunk() {
    auto chunk = chunks.front();
    chunks.pop();
    return chunk;
}

void ChunkedResponse::pushChunk(const std::string &chunk) {
    chunks.push(chunk);
}
