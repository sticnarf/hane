#ifndef HANE_FINAL_PARSER_HPP
#define HANE_FINAL_PARSER_HPP

#include "abstract_parser.hpp"

class FinalParser : public AbstractParser {
public:
    FinalParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
