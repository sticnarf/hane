#ifndef RACKCPP_BODY_FORM_PARSER_HPP
#define RACKCPP_BODY_FORM_PARSER_HPP

#include "abstract_parser.hpp"

class BodyFormParser : public AbstractParser {

public:
    BodyFormParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
