#ifndef HANE_PARSER_HPP
#define HANE_PARSER_HPP

#include "../request/request.hpp"
#include "../../utils/buffer.hpp"
#include "./abstract_parser.hpp"
#include <queue>

class Parser {
private:
    BufferPtr buffer;
    ParserPtr currentParser;
    std::queue<Request> completeRequests;

    void process();

public:

    Parser();

    void pushBuf(const char *buf, size_t len);

    bool hasCompleteRequest();

    Request yieldRequest();
};

#endif
