#ifndef HANE_TRUNKED_BODY_PARSER_HPP
#define HANE_TRUNKED_BODY_PARSER_HPP

#include "./abstract_parser.hpp"

class TrunkedBodyParser : public AbstractParser {
public:
    TrunkedBodyParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
