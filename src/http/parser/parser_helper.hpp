#ifndef HANE_PARSER_HELPERS_HPP
#define HANE_PARSER_HELPERS_HPP

#include "../request/request.hpp"
#include "../request/header.hpp"
#include "abstract_parser.hpp"
#include <string>
#include <utility>

class ParserHelper {
public:
    static void parseUrlEncodedQueries(const std::string &data, Request &req, size_t begin = 0);

    static void parseParameters(const std::string &data, FieldParameters &req, size_t begin = 0);

    static std::pair<std::string, std::string> parseHeaderField(const std::string &data, size_t begin = 0);

    static bool validateHeaderFieldContent(const std::string &content);

    static bool validateToken(const std::string &name);

    static void parseHeaderFieldWithParameters(HeaderContentWithParametersPtr content, const std::string &fieldContent);

    static ParserPtr buildFormParser(Request &&partialRequest, BufferPtr buffer);

    static ParserPtr buildBodyParser(Request &&partialRequest, BufferPtr buffer);
};

#endif
