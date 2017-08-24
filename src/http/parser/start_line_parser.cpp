#include <stdexcept>
#include "../errors.hpp"
#include "../../utils/protocol_helper.hpp"
#include "start_line_parser.hpp"
#include "header_parser.hpp"
#include "url_queries_parser.hpp"

StartLineParser::StartLineParser(RequestPtr req, BufferPtr buffer)
        : AbstractParser(req, buffer) {}

ParserPtr StartLineParser::process() {
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::make_shared<StartLineParser>(std::move(partialRequest), buffer);
    std::string startLine = buffer->split(lineSep + 2)->toString(0, lineSep);

    size_t sp1 = startLine.find(' ');
    size_t sp2 = startLine.find(' ', sp1 + 1);
    if (sp1 == std::string::npos || sp2 == std::string::npos)
        throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Bad start line");

    HttpMethod method = toHttpMethod(startLine.data(), sp1);

    std::string target = startLine.substr(sp1 + 1, sp2 - sp1 - 1);

    HttpVersion version = toHttpVersion(startLine.data() + sp2 + 1, lineSep - sp2 - 1);

    // If no exception is thrown, the format is OK.
    partialRequest->httpMethod = method;
    partialRequest->target = target;
    partialRequest->httpVersion = version;

    return UrlQueriesParser(std::move(partialRequest), buffer).process();
}
