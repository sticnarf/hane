#ifndef RACKCPP_QUERIES_PARSER_CPP_H
#define RACKCPP_QUERIES_PARSER_CPP_H
#include "abstract_parser.h"

class QueriesParser: public AbstractParser
{
public:
    QueriesParser(Request&& req, std::shared_ptr<Buffer> buffer);
    std::shared_ptr<AbstractParser> process();
};

#endif
