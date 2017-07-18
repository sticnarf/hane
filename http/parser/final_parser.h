#ifndef RACKCPP_FINAL_PARSER_H
#define RACKCPP_FINAL_PARSER_H
#include "abstract_parser.h"

class FinalParser : public AbstractParser
{
public:
    FinalParser(Request&& req, Buffer& buffer);
    std::shared_ptr<AbstractParser> process();
};

#endif
