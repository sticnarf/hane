#ifndef HANE_START_LINE_PARSER_HPP
#define HANE_START_LINE_PARSER_HPP

#include "./abstract_parser.hpp"

class StartLineParser : public AbstractParser {
public:
    StartLineParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
