#include "sized_body_parser.h"
#include "final_parser.h"
#include "body_form_parser.h"

SizedBodyParser::SizedBodyParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr SizedBodyParser::process()
{
    auto contentLengthEntry = partialRequest.header.get("Content-Length");
    int contentLength;
    if (contentLengthEntry.isValid()
            // Throws std::invalid_argument if conversion fails
            // TODO: Not handle overflow
            && (contentLength = std::stoi(contentLengthEntry.getValue().getContent())) >= 0)
    {
        if (buffer->len() < contentLength)
        {
            return std::shared_ptr<AbstractParser>(this);
        }

        partialRequest.body = buffer->split((size_t) contentLength);
    }
    else
    {
        // Empty body
        partialRequest.body = std::make_shared<Buffer>();
    }

    // application/x-www-form-urlencoded
    // TODO Not supporting charset parameter
    auto contentType = partialRequest.header.get("Content-Type");
    if (contentType.isValid() && contentType.getValue().getContent() == CONTENT_TYPE_URLENCODED_FORM)
    {
        return BodyFormParser(std::move(partialRequest), buffer).process();
    }

    return buildFormParser()->process();
}

ParserPtr SizedBodyParser::buildFormParser()
{
    auto contentTypeEntry = partialRequest.header.get("Content-Type");
    if (!contentTypeEntry.isValid())
    {
        // Unknown!
        return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
    }
    auto contentType = contentTypeEntry.getValue().getContent();


    // application/x-www-form-urlencoded
    // TODO Not supporting charset parameter
    if (contentType == CONTENT_TYPE_URLENCODED_FORM)
    {
        return std::make_shared<BodyFormParser>(std::move(partialRequest), buffer);
    }

    // multipart/form-data
    // TODO
//    if (contentType == )

    // Unknown!
    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
