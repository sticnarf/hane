#ifndef HANE_TRUNKED_BODY_PARSER_HPP
#define HANE_TRUNKED_BODY_PARSER_HPP

#include "./abstract_parser.hpp"

class ChunkedBodyParser : public AbstractParser {
public:
    ChunkedBodyParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
