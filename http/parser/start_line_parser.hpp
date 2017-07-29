#ifndef RACKCPP_START_LINE_PARSER_H
#define RACKCPP_START_LINE_PARSER_H
#include "abstract_parser.hpp"

class StartLineParser: public AbstractParser
{
public:
    StartLineParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
