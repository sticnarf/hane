#include "header_parser.hpp"
#include "sized_body_parser.hpp"
#include <functional>
#include <utility>
#include "http/request/header_fields/content_type.hpp"
#include "parser_helper.hpp"

HeaderParser::HeaderParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), std::move(buffer)) {}

ParserPtr HeaderParser::process() {
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::shared_ptr<AbstractParser>(this);

    auto fieldBuf = buffer->split(lineSep + 2);
    if (lineSep == 0)
        return buildBodyParser()->process();

    std::string headerField = fieldBuf->toString(0, lineSep);

    auto parseResult = ParserHelper::parseHeaderField(headerField);
    auto fieldName = parseResult.first;
    auto fieldContent = parseResult.second;

    partialRequest.header.put(fieldName, parseField(fieldName, fieldContent));

    return this->process();
}

ParserPtr HeaderParser::buildBodyParser() {
    // TODO Build different BodyParser according to Transfer-Encoding
    return std::make_shared<SizedBodyParser>(std::move(partialRequest), buffer);
}

HeaderContentPtr parseContentType(const std::string &fieldContent) {
    auto contentType = std::make_shared<ContentType>();

    ParserHelper::parseHeaderFieldWithParameters(contentType, fieldContent);

    return contentType;
}

HeaderContentPtr HeaderParser::parseField(const std::string &fieldName, const std::string &fieldContent) {
    // TODO Add other header fields
    static std::map<std::string, std::function<HeaderContentPtr(const std::string &)>> functionMap = {
            {"content-type", parseContentType}
    };

    auto function = functionMap.find(StringUtils::toLowercase(fieldName));
    if (function != functionMap.end()) {
        return function->second(fieldContent);
    }

    return std::make_shared<HeaderContent>(fieldContent);
}
