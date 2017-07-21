#ifndef RACKCPP_BODY_PARSER_H
#define RACKCPP_BODY_PARSER_H
#include "abstract_parser.h"

class SizedBodyParser : public AbstractParser
{
public:
    SizedBodyParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
