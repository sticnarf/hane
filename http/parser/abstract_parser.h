#ifndef RACKCPP_ABSTRACT_PARSER_H
#define RACKCPP_ABSTRACT_PARSER_H
#include "http/request/request.h"
#include "utils/buffer.h"

class AbstractParser
{
protected:
    Request partialRequest;
    Buffer& buffer;

    AbstractParser(Request&& req, Buffer& buffer);

public:
    virtual AbstractParser process();
};

#endif
