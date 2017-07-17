#ifndef RACKCPP_BODY_PARSER_H
#define RACKCPP_BODY_PARSER_H
#include "abstract_parser.h"

class RawBodyParser : public AbstractParser
{
public:
    RawBodyParser(Request&& req, Buffer& buffer);
    std::unique_ptr<AbstractParser> process();
};

#endif
