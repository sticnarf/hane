#include <stdexcept>
#include "utils/protocol_helper.h"
#include "start_line_parser.h"
#include "header_parser.h"
#include "queries_parser.h"

StartLineParser::StartLineParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr StartLineParser::process()
{
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::shared_ptr<AbstractParser>(this);
    std::string startLine = buffer->split(lineSep + 2)->toString(0, lineSep);

    size_t sp1 = startLine.find(' ');
    size_t sp2 = startLine.find(' ', sp1 + 1);
    if (sp1 == std::string::npos || sp2 == std::string::npos)
        throw std::invalid_argument("Bad start line");

    HttpMethod method = toHttpMethod(startLine.data(), sp1);

    std::string target = startLine.substr(sp1 + 1, sp2 - sp1 - 1);

    HttpVersion version = toHttpVersion(startLine.data() + sp2 + 1, lineSep - sp2 - 1);

    // If no exception is thrown, the format is OK.
    partialRequest.httpMethod = method;
    partialRequest.target = target;
    partialRequest.httpVersion = version;

    return QueriesParser(std::move(partialRequest), buffer).process();
}
