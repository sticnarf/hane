#ifndef RACKCPP_FINAL_PARSER_H
#define RACKCPP_FINAL_PARSER_H
#include "abstract_parser.hpp"

class FinalParser : public AbstractParser
{
public:
    FinalParser(Request&& req, BufferPtr buffer);
    ParserPtr process() override;
};

#endif
