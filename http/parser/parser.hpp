#ifndef RACKCPP_PARSER_H
#define RACKCPP_PARSER_H

#include "http/request/request.hpp"
#include "utils/buffer.hpp"
#include "abstract_parser.hpp"
#include <queue>

class Parser
{
private:
    BufferPtr buffer;
    ParserPtr currentParser;
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

    Parser();
    void pushBuf(const char* buf, size_t len);
    bool hasCompleteRequest();
    Request yieldRequest();
};

#endif
