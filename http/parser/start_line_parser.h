#ifndef RACKCPP_START_LINE_PARSER_H
#define RACKCPP_START_LINE_PARSER_H
#include "abstract_parser.h"

class StartLineParser: public AbstractParser
{
public:
    StartLineParser(Request&& req, std::shared_ptr<Buffer> buffer);
    std::shared_ptr<AbstractParser> process();
};

#endif
