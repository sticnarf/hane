#ifndef HANE_BODY_PARSER_HPP
#define HANE_BODY_PARSER_HPP

#include "./abstract_parser.hpp"

class SizedBodyParser : public AbstractParser {
public:
    SizedBodyParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
