#ifndef RACKCPP_PARSER_H
#define RACKCPP_PARSER_H

#include "http/request/request.h"
#include "utils/buffer.h"
#include <queue>

class Parser
{
private:
    Buffer buffer;
    Request partialRequest;
    std::queue<Request> completeRequests;

    void process();
public:
    enum class Stage
    {
        REQUEST_LINE,
        HEADER_FIELDS,
        MESSAGE_BODY,
        BODY_PROCESSING,
        PARSING_FINISHED
    } stage = Stage::REQUEST_LINE;

    void pushBuf(const char* buf, size_t len);
    bool hasCompleteRequest();
    Request yieldRequest();
};

#endif
