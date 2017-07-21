#ifndef RACKCPP_FINAL_PARSER_H
#define RACKCPP_FINAL_PARSER_H
#include "abstract_parser.h"

class FinalParser : public AbstractParser
{
public:
    FinalParser(Request&& req, std::shared_ptr<Buffer> buffer);
};

#endif
