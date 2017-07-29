#ifndef RACKCPP_ABSTRACT_PARSER_H
#define RACKCPP_ABSTRACT_PARSER_H
#include "http/request/request.hpp"
#include "utils/buffer.hpp"
#include <memory>

class AbstractParser;

typedef std::shared_ptr<AbstractParser> ParserPtr;

class AbstractParser
{
protected:
    Request partialRequest;
    BufferPtr buffer;

    bool finished;

    AbstractParser(Request&& req, BufferPtr buffer);

public:
    // Throws std::invalid_argument if it is a bad request
    virtual ParserPtr process();

    bool isFinished() const;
    Request getRequest() const;
};

#endif
