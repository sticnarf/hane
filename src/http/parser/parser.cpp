#include "parser.hpp"
#include "start_line_parser.hpp"

void Parser::pushBuf(const char *buf, size_t len) {
    buffer->push(buf, len);
    process();
}

RequestPtr Parser::yieldRequest() {
    RequestPtr req = completeRequests.front();
    completeRequests.pop();
    return req;
}

bool Parser::hasCompleteRequest() {
    return !completeRequests.empty();
}

void Parser::process() {
    currentParser = currentParser->process();

    if (currentParser->isFinished()) {
        completeRequests.push(currentParser->getRequest());
        currentParser = std::make_shared<StartLineParser>(std::make_shared<Request>(), buffer);
    }
}

Parser::Parser() {
    buffer = std::make_shared<Buffer>();
    currentParser = std::make_shared<StartLineParser>(std::make_shared<Request>(), buffer);
}
