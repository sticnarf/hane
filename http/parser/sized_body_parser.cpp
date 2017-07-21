#include "sized_body_parser.h"
#include "final_parser.h"

SizedBodyParser::SizedBodyParser(Request&& req, std::shared_ptr<Buffer> buffer)
        :AbstractParser(std::move(req), buffer) { }

std::shared_ptr<AbstractParser> SizedBodyParser::process()
{
    auto contentLengthEntry = partialRequest.header.getFieldEntry("Content-Length");
    int contentLength;
    if (contentLengthEntry.isValid()
            // Throws std::invalid_argument if conversion fails
            // TODO: Not handle overflow
            && (contentLength = std::stoi(contentLengthEntry.getHeaderField().getContent())) >= 0)
    {
        if (buffer->len() < contentLength)
        {
            return std::shared_ptr<AbstractParser>(this);
        }

        partialRequest.body = buffer->split((size_t) contentLength);
    }
    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
