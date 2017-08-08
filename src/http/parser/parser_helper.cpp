#include "parser_helper.hpp"
#include <cctype>
#include <cstring>
#include "../request/header_fields/content_type.hpp"
#include "../errors.hpp"
#include "abstract_parser.hpp"
#include "final_parser.hpp"
#include "body_form_parser.hpp"
#include "multipart_form_parser.hpp"
#include "sized_body_parser.hpp"
#include "trunked_body_parser.hpp"

void ParserHelper::parseUrlEncodedQueries(const std::string &data, Request &req, size_t begin) {
    bool eof = false;
    while (!eof) {
        size_t equalSign = data.find('=', begin);
        if (equalSign == std::string::npos)
            break;
        std::string key, val;
        for (size_t i = begin; i < equalSign; i++) {
            if (data[i] == '%') {
                key += (char) std::stoi(data.substr(i + 1, 2), 0, 16);
                i += 2;
            } else {
                key += data[i];
            }
        }

        size_t ampersand = data.find('&', equalSign);
        eof = ampersand == std::string::npos;
        size_t end = eof ? data.size() : ampersand;
        for (size_t i = equalSign + 1; i < end; i++) {
            if (data[i] == '%') {
                val += (char) std::stoi(data.substr(i + 1, 2), 0, 16);
                i += 2;
            } else {
                val += data[i];
            }
        }

        req.queries.insert({key, val});
    }
}

// TODO Not supporting escape characters between quotes
void ParserHelper::parseParameters(const std::string &data, FieldParameters &parameters, size_t begin) {
    bool eof = false;
    while (!eof) {
        size_t equalSign = data.find('=', begin);
        if (equalSign == std::string::npos)
            break;

        std::string key, val;
        key = data.substr(begin, equalSign - begin);
        StringUtils::trim(key);
        if (!validateToken(key))
            throw HttpError(StatusCode::HTTP_BAD_REQUEST, "invalid header parameter");

        size_t valueBegin = equalSign + 1;
        while (isspace(data[valueBegin]))
            valueBegin++;

        if (data[valueBegin] == '"' && ++valueBegin < data.length()) {
            // Quoted string
            size_t rightQuote = data.find('"', valueBegin);
            if (rightQuote == std::string::npos)
                throw std::invalid_argument("invalid header value");

            val = data.substr(valueBegin, rightQuote - valueBegin);

            size_t semicolon = data.find(';', rightQuote);
            eof = semicolon == std::string::npos;
            begin = semicolon + 1;
        } else {
            // token
            size_t semicolon = data.find(';', equalSign);
            eof = semicolon == std::string::npos;
            size_t end = eof ? data.size() : semicolon;
            begin = semicolon + 1;

            val = data.substr(equalSign + 1, end - equalSign - 1);
            StringUtils::trim(val);
            if (!validateToken(val))
                throw HttpError(StatusCode::HTTP_BAD_REQUEST, "invalid header value");
        }

        parameters.put(key, val);
    }
}

bool ParserHelper::validateToken(const std::string &name) {
    static const char *list = "!#$%&'*+-.^_`|~";
    static const size_t listLen = strlen(list);

    if (name.length() == 0)
        return false;

    for (char c : name) {
        if (isalnum(c))
            continue;

        bool ok = false;
        for (size_t i = 0; i < listLen; i++) {
            if (c == list[i]) {
                ok = true;
                break;
            }
        }

        if (!ok)
            return false;
    }

    return true;
}

bool ParserHelper::validateHeaderFieldContent(const std::string &content) {
    for (char c : content) {
        if (!isprint(c))
            return false;
    }

    return true;
}

std::pair<std::string, std::string> ParserHelper::parseHeaderField(const std::string &data, size_t begin) {
    size_t colon = data.find(':', begin);
    std::string fieldName = data.substr(begin, colon);
    if (!ParserHelper::validateToken(fieldName))
        throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Bad field name");

    size_t firstNotSpace = colon + 1;
    while (isspace(data[firstNotSpace]))
        firstNotSpace++;

    size_t lastNotSpace = data.length() - 1;
    while (isspace(data[lastNotSpace]))
        lastNotSpace--;

    std::string fieldContent;
    try {
        fieldContent = StringUtils::processQuotedPair(
                data.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1));
    } catch (const std::invalid_argument &_) {
        throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Bad field content");
    }
    if (!ParserHelper::validateHeaderFieldContent(fieldContent))
        throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Bad field content");

    return std::make_pair(fieldName, fieldContent);
}

void ParserHelper::parseHeaderFieldWithParameters(HeaderContentWithParametersPtr content,
                                                  const std::string &fieldContent) {
    content->content = fieldContent;
    auto semicolon = fieldContent.find(';');

    // Use feature of substr: if the second argument of substr is npos, it will returns [pos, size())
    content->mainContent = fieldContent.substr(0, semicolon);
    StringUtils::trim(content->mainContent);

    // Parse the remaining parameters
    if (semicolon != std::string::npos)
        ParserHelper::parseParameters(fieldContent, content->parameters, semicolon + 1);
}

ParserPtr ParserHelper::buildFormParser(Request &&partialRequest, BufferPtr buffer) {
    auto contentTypeEntry = partialRequest.header.get("Content-Type");
    if (!contentTypeEntry.isValid()) {
        // Unknown!
        return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
    }
    auto contentType = std::dynamic_pointer_cast<ContentType>(contentTypeEntry.getValue());
    auto mediaType = contentType->getLowercasedMediaType();

    // application/x-www-form-urlencoded
    // TODO Not supporting charset parameter
    if (mediaType == CONTENT_TYPE_URLENCODED_FORM) {
        return std::make_shared<BodyFormParser>(std::move(partialRequest), buffer);
    }

    // multipart/form-data
    if (mediaType == CONTENT_TYPE_FORM_MULTIPART) {
        return std::make_shared<MultipartFormParser>(std::move(partialRequest), buffer);
    }

    // Unknown!
    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}

ParserPtr ParserHelper::buildBodyParser(Request &&partialRequest, BufferPtr buffer) {
    // TODO Build different BodyParser according to Transfer-Encoding
    auto transferEncoding = partialRequest.header.get("Transfer-Encoding");
    if (transferEncoding.isValid()) {
        if (transferEncoding.getValue()->getContent() == TRANSFER_ENCODING_TRUNKED) {
            // trunked encoding
            return std::make_shared<TrunkedBodyParser>(std::move(partialRequest), buffer);
        } else {
            throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Unsupported Transfer-Encoding");
        }
    } else {
        // With Content-Length
        return std::make_shared<SizedBodyParser>(std::move(partialRequest), buffer);
    }
}
