#include "queries_parser.h"
#include "header_parser.h"
#include "parser_helpers.h"

QueriesParser::QueriesParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr QueriesParser::process()
{
    const std::string& target = partialRequest.target;
    size_t begin = target.find_last_of('?');

    if (begin != std::string::npos && begin != target.length() - 1)
        ParserHelper::parseUrlEncodedQueries(target, partialRequest, begin + 1);

    return HeaderParser(std::move(partialRequest), buffer).process();
}