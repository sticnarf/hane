#ifndef RACKCPP_BODY_PARSER_HPP
#define RACKCPP_BODY_PARSER_HPP

#include "abstract_parser.hpp"

class SizedBodyParser : public AbstractParser {
    ParserPtr buildFormParser();

public:
    SizedBodyParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
