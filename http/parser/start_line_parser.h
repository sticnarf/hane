#ifndef RACKCPP_START_LINE_PARSER_H
#define RACKCPP_START_LINE_PARSER_H
#include "abstract_parser.h"

class StartLineParser : AbstractParser
{
public:
    StartLineParser(Request&& req, Buffer& buffer);
    AbstractParser process();
};

#endif
