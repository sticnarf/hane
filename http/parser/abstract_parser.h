#ifndef RACKCPP_ABSTRACT_PARSER_H
#define RACKCPP_ABSTRACT_PARSER_H
#include "http/request/request.h"
#include "utils/buffer.h"
#include <memory>

class AbstractParser
{
protected:
    Request partialRequest;
    std::shared_ptr<Buffer> buffer;

    bool finished;

    AbstractParser(Request&& req, std::shared_ptr<Buffer> buffer);

public:
    // Throws std::invalid_argument if it is a bad request
    virtual std::shared_ptr<AbstractParser> process();

    bool isFinished() const;
    Request getRequest() const;
};

#endif
