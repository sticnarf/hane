#include "header_parser.h"
#include "sized_body_parser.h"
#include <functional>
#include "http/request/header_fields/content_type.h"
#include "parser_helper.h"

HeaderParser::HeaderParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr HeaderParser::process()
{
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::shared_ptr<AbstractParser>(this);

    auto fieldBuf = buffer->split(lineSep + 2);
    if (lineSep == 0)
    {
        return buildBodyParser()->process();
    }

    std::string headerField = fieldBuf->toString(0, lineSep);

    size_t colon = headerField.find(':');
    std::string fieldName = headerField.substr(0, colon);
    if (!ParserHelper::validateToken(fieldName))
        throw std::invalid_argument("Bad field name");

    size_t firstNotSpace = colon + 1;
    while (isspace(headerField[firstNotSpace]))
        firstNotSpace++;

    size_t lastNotSpace = headerField.length() - 1;
    while (isspace(headerField[lastNotSpace]))
        lastNotSpace--;

    std::string fieldContent = headerField.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1);
    if (!ParserHelper::validateHeaderFieldContent(fieldContent))
        throw std::invalid_argument("Bad field content");

    // This is OK for general header fields
    // TODO Polymorphism for special header fields
    partialRequest.header.put(fieldName, parseField(fieldName, fieldContent));

    return this->process();
}

ParserPtr HeaderParser::buildBodyParser()
{
    // TODO Build different BodyParser according to Transfer-Encoding
    return std::make_shared<SizedBodyParser>(std::move(partialRequest), buffer);
}

HeaderContentPtr parseContentType(const std::string& fieldContent)
{
    auto contentType = std::make_shared<ContentType>();

    auto semicolon = fieldContent.find(';');

    // Feature: if the second argument of substr is npos, it will returns [pos, size())
    contentType->mediaType = fieldContent.substr(0, semicolon);
    StringUtils::trim(contentType->mediaType);
    if (!ParserHelper::validateToken(contentType->mediaType))
        throw std::invalid_argument("Bad media type");

    // Parse the remaining parameters
    if (semicolon != std::string::npos)
    {
        ParserHelper::parseParameters(fieldContent, contentType->parameters, semicolon + 1);
    }

    return contentType;
}

HeaderContentPtr HeaderParser::parseField(const std::string& fieldName, const std::string& fieldContent)
{
    // TODO Add other header fields
    static std::map<std::string, std::function<HeaderContentPtr(const std::string&)>> functionMap = {
            {"content-type", parseContentType}
    };

    auto function = functionMap.find(StringUtils::toLowercase(fieldContent));
    if (function != functionMap.end())
    {
        return function->second(fieldContent);
    }

    return std::make_shared<HeaderContent>(fieldContent);
}
