#ifndef RACKCPP_HEADER_PARSER_H
#define RACKCPP_HEADER_PARSER_H

#include "abstract_parser.h"

class HeaderParser : public AbstractParser
{
    std::shared_ptr<AbstractParser> buildBodyParser();

public:
    HeaderParser(Request&& req, std::shared_ptr<Buffer> buffer);
    std::shared_ptr<AbstractParser> process();
};

#endif
