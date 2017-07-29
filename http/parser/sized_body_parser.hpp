#ifndef RACKCPP_BODY_PARSER_H
#define RACKCPP_BODY_PARSER_H
#include "abstract_parser.hpp"

class SizedBodyParser : public AbstractParser
{
    ParserPtr buildFormParser();
public:
    SizedBodyParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
