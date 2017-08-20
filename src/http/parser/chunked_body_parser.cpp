#include "chunked_body_parser.hpp"
#include "parser_helper.hpp"

#include <utility>
#include <http/errors.hpp>

ChunkedBodyParser::ChunkedBodyParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), std::move(buffer)) {}

ParserPtr ChunkedBodyParser::process() {
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::make_shared<ChunkedBodyParser>(std::move(partialRequest), buffer);

    auto lengthString = buffer->toString(0, lineSep);
    auto chunkLength = std::stoul(lengthString, 0, 16);
    if (chunkLength + lineSep + 4 > buffer->len())
        return std::make_shared<ChunkedBodyParser>(std::move(partialRequest), buffer);

    // TODO Cannot push a buffer into another buffer. Try avoid useless copy!

    buffer->split(lineSep + 2); // Remove the line that indicates the length
    auto chunkData = buffer->split(chunkLength)->toString();
    auto chunkEnd = buffer->split(2);
    if (chunkEnd->toString() != "\r\n")
        throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Bad chunk");


    partialRequest.body->push(chunkData.data(), chunkData.length());

    if (chunkLength == 0)
        return ParserHelper::buildFormParser(std::move(partialRequest), buffer)->process();

    return std::make_shared<ChunkedBodyParser>(std::move(partialRequest), buffer)->process();
}
