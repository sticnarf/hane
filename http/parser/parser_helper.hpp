#ifndef RACKCPP_PARSER_HELPERS_H
#define RACKCPP_PARSER_HELPERS_H

#include "http/request/request.hpp"
#include "http/request/header.hpp"
#include <string>
#include <utility>

class ParserHelper
{
public:
    static void parseUrlEncodedQueries(const std::string& data, Request& req, size_t begin = 0);
    static void parseParameters(const std::string& data, FieldParameters& req, size_t begin = 0);
    static std::pair<std::string, std::string> parseHeaderField(const std::string& data, size_t begin = 0);
    static bool validateHeaderFieldContent(const std::string& content);
    static bool validateToken(const std::string& name);
    static void parseHeaderFieldWithParameters(HeaderContentWithParametersPtr content, const std::string& fieldContent);
};

#endif
