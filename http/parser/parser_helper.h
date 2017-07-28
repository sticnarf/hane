#ifndef RACKCPP_PARSER_HELPERS_H
#define RACKCPP_PARSER_HELPERS_H

#include "http/request/request.h"
#include "http/request/header.h"
#include <string>

class ParserHelper
{
public:
    static void parseUrlEncodedQueries(const std::string& data, Request& req, size_t begin = 0);
    static void parseParameters(const std::string& data, FieldParameters& req, size_t begin = 0);
    static bool validateHeaderFieldContent(const std::string& content);
    static bool validateToken(const std::string& name);
};

#endif
