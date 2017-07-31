#ifndef HANE_ABSTRACT_PARSER_HPP
#define HANE_ABSTRACT_PARSER_HPP

#include "http/request/request.hpp"
#include "utils/buffer.hpp"
#include <memory>

class AbstractParser;

typedef std::shared_ptr<AbstractParser> ParserPtr;

class AbstractParser {
protected:
    Request partialRequest;
    BufferPtr buffer;

    bool finished;

    AbstractParser(Request &&req, BufferPtr buffer);

public:
    // Throws HttpError if it is a bad request
    virtual ParserPtr process();

    bool isFinished() const;

    Request getRequest() const;
};

#endif
