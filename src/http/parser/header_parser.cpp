#include "header_parser.hpp"
#include "sized_body_parser.hpp"
#include <functional>
#include <utility>
#include <http/errors.hpp>
#include "http/header/content_type.hpp"
#include "parser_helper.hpp"

HeaderParser::HeaderParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), buffer) {}

ParserPtr HeaderParser::process() {
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::make_shared<HeaderParser>(std::move(partialRequest), buffer);

    auto fieldBuf = buffer->split(lineSep + 2);
    if (lineSep == 0)
        return ParserHelper::buildBodyParser(std::move(partialRequest), buffer)->process();

    std::string headerField = fieldBuf->toString(0, lineSep);

    auto parseResult = ParserHelper::parseHeaderField(headerField);
    auto fieldName = parseResult.first;
    auto fieldContent = parseResult.second;

    partialRequest.header.put(fieldName, parseField(fieldName, fieldContent));

    return this->process();
}

HeaderContentPtr parseContentType(const std::string &fieldContent) {
    auto contentType = std::make_shared<ContentType>();

    ParserHelper::parseHeaderFieldWithParameters(contentType, fieldContent);

    return contentType;
}

HeaderContentPtr parseCookies(const std::string &fieldContent) {
    auto cookies = std::make_shared<Cookies>();

    ParserHelper::parseCookies(cookies, fieldContent);

    return cookies;
}

HeaderContentPtr HeaderParser::parseField(const std::string &fieldName, const std::string &fieldContent) {
    // TODO Add other header fields
    static std::map<std::string, std::function<HeaderContentPtr(const std::string &)>> functionMap = {
            {"content-type", parseContentType},
            {"cookie",       parseCookies},
    };

    auto function = functionMap.find(StringUtils::toLowercase(fieldName));
    if (function != functionMap.end()) {
        return function->second(fieldContent);
    }

    return std::make_shared<HeaderContent>(fieldContent);
}
