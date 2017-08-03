#ifndef HANE_QUERIES_PARSER_CPP_HPP
#define HANE_QUERIES_PARSER_CPP_HPP

#include "./abstract_parser.hpp"

class QueriesParser : public AbstractParser {
public:
    QueriesParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
