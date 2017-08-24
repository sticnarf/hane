#include "url_queries_parser.hpp"

#include <utility>
#include "header_parser.hpp"
#include "parser_helper.hpp"

UrlQueriesParser::UrlQueriesParser(RequestPtr req, BufferPtr buffer)
        : AbstractParser(req, buffer) {}

ParserPtr UrlQueriesParser::process() {
    const std::string &target = partialRequest->target;
    size_t begin = target.find_last_of('?');

    if (begin != std::string::npos && begin != target.length() - 1)
        ParserHelper::parseUrlEncodedQueries(target, partialRequest, begin + 1);
    partialRequest->absPath = target.substr(0, begin);

    return HeaderParser(std::move(partialRequest), buffer).process();
}