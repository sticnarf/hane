#ifndef RACKCPP_PARSER_HELPERS_H
#define RACKCPP_PARSER_HELPERS_H

#include "http/request/request.h"
#include <string>

class ParserHelper
{
public:
    static void parseUrlEncodedQueries(const std::string& data, Request& req, size_t begin = 0);
};

#endif
