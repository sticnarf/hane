#ifndef RACKCPP_QUERIES_PARSER_CPP_H
#define RACKCPP_QUERIES_PARSER_CPP_H
#include "abstract_parser.hpp"

class QueriesParser: public AbstractParser
{
public:
    QueriesParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
