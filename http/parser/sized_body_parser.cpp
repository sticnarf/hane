#include <http/request/header_fields/content_type.hpp>
#include <utility>
#include "sized_body_parser.hpp"
#include "final_parser.hpp"
#include "body_form_parser.hpp"
#include "multipart_form_parser.hpp"

SizedBodyParser::SizedBodyParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), std::move(buffer)) {}

ParserPtr SizedBodyParser::process() {
    auto contentLengthEntry = partialRequest.header.get("Content-Length");
    size_t contentLength;
    if (contentLengthEntry.isValid()
        // Throws std::invalid_argument if conversion fails
        // TODO: Not handle overflow
        && (contentLength = std::stoul(contentLengthEntry.getValue()->getContent())) >= 0) {
        if (buffer->len() < contentLength) {
            return std::shared_ptr<AbstractParser>(this);
        }

        partialRequest.body = buffer->split((size_t) contentLength);
    } else {
        // Empty body
        partialRequest.body = std::make_shared<Buffer>();
    }

    // application/x-www-form-urlencoded
    // TODO Not supporting charset parameter
    auto contentType = partialRequest.header.get("Content-Type");
    if (contentType.isValid() && contentType.getValue()->getContent() == CONTENT_TYPE_URLENCODED_FORM) {
        return BodyFormParser(std::move(partialRequest), buffer).process();
    }

    return buildFormParser()->process();
}

ParserPtr SizedBodyParser::buildFormParser() {
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
    // TODO
    if (mediaType == CONTENT_TYPE_FORM_MULTIPART) {
        return std::make_shared<MultipartFormParser>(std::move(partialRequest), buffer);
    }

    // Unknown!
    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
