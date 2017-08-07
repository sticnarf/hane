#ifndef HANE_QUERIES_PARSER_CPP_HPP
#define HANE_QUERIES_PARSER_CPP_HPP

#include "./abstract_parser.hpp"

class UrlQueriesParser : public AbstractParser {
public:
    UrlQueriesParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
