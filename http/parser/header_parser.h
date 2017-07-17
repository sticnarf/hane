#ifndef RACKCPP_HEADER_PARSER_H
#define RACKCPP_HEADER_PARSER_H

#include "abstract_parser.h"

class HeaderParser : public AbstractParser
{
    std::unique_ptr<AbstractParser> buildBodyParser();

public:
    HeaderParser(Request&& req, Buffer& buffer);
    std::unique_ptr<AbstractParser> process();
};

#endif
