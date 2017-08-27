#ifndef HANE_PARSER_HPP
#define HANE_PARSER_HPP

#include "../request/request.hpp"
#include "../../utils/buffer.hpp"
#include "./abstract_parser.hpp"
#include "../request/bad_request.hpp"
#include "start_line_parser.hpp"
#include "../errors.hpp"
#include <queue>
#include <mutex>

class Client;

class Parser {
private:
    BufferPtr buffer;
    ParserPtr currentParser;
    std::queue<RequestPtr> completeRequests;
    std::mutex parserMutex;
    Client *client;

    void process();

public:
    Parser(Client *);

    void pushBuf(const char *buf, size_t len);

    bool hasCompleteRequest();

    RequestPtr yieldRequest();

    friend class Client;
};

#endif
