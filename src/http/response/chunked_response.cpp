#include "chunked_response.hpp"

ChunkedResponse::ChunkedResponse(HttpVersion version) : Response(version) {
    headers.put("Transfer-Encoding", std::make_shared<HeaderContent>("chunked"));
}

ChunkedResponse::ChunkedResponse(const Response &resp) : Response(resp) {
    chunks.push(resp.body);
    headers.put("Transfer-Encoding", std::make_shared<HeaderContent>("chunked"));
}

bool ChunkedResponse::empty() {
    return chunks.empty();
}

std::string ChunkedResponse::popChunk() {
    auto &chunk = chunks.front();
    chunks.pop();
    return chunk;
}

void ChunkedResponse::pushChunk(std::string &&chunk) {
    chunks.push(chunk);
}
