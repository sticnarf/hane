#ifndef HANE_BODY_FORM_PARSER_HPP
#define HANE_BODY_FORM_PARSER_HPP

#include "./abstract_parser.hpp"

class BodyFormParser : public AbstractParser {

public:
    BodyFormParser(RequestPtr req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
