#include "parser.hpp"

void Parser::pushBuf(const char *buf, size_t len) {
    buffer->push(buf, len);
    process();
}

RequestPtr Parser::yieldRequest() {
    std::lock_guard<std::mutex> lk(parserMutex);
    RequestPtr req = completeRequests.front();
    completeRequests.pop();
    return req;
}

bool Parser::hasCompleteRequest() {
    return !completeRequests.empty();
}

Parser::Parser(Client *client) : client(client) {
    buffer = std::make_shared<Buffer>();
    currentParser = std::make_shared<StartLineParser>(std::make_shared<Request>(), buffer);
}
