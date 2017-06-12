#ifndef RACKCPP_PARSER_H
#define RACKCPP_PARSER_H

#include "http/request.h"
#include "utils/buffer.h"

class Parser
{
private:
    Buffer buffer;

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
    Request yieldRequest();
};

#endif
