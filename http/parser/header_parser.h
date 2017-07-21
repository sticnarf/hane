#ifndef RACKCPP_HEADER_PARSER_H
#define RACKCPP_HEADER_PARSER_H

#include "abstract_parser.h"

class HeaderParser : public AbstractParser
{
    ParserPtr buildBodyParser();

public:
    HeaderParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
