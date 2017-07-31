#ifndef RACKCPP_START_LINE_PARSER_HPP
#define RACKCPP_START_LINE_PARSER_HPP

#include "abstract_parser.hpp"

class StartLineParser : public AbstractParser {
public:
    StartLineParser(Request &&req, BufferPtr buffer);

    ParserPtr process();
};

#endif
