#include "http/header/content_type.hpp"
#include <utility>
#include "sized_body_parser.hpp"
#include "final_parser.hpp"
#include "body_form_parser.hpp"
#include "multipart_form_parser.hpp"
#include "parser_helper.hpp"

SizedBodyParser::SizedBodyParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), std::move(buffer)) {}

ParserPtr SizedBodyParser::process() {
    auto contentLengthEntry = partialRequest.header.get("Content-Length");
    size_t contentLength;
    if (contentLengthEntry.isValid()) {
        // Throws std::invalid_argument if conversion fails
        // TODO: Not handle overflow
        contentLength = std::stoul(contentLengthEntry.getValue()->getContent());
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

    return ParserHelper::buildFormParser(std::move(partialRequest), buffer)->process();
}
