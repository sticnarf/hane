#include "trunked_body_parser.hpp"
#include "parser_helper.hpp"

#include <utility>
#include <http/errors.hpp>

TrunkedBodyParser::TrunkedBodyParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), std::move(buffer)) {}

ParserPtr TrunkedBodyParser::process() {
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::make_shared<TrunkedBodyParser>(std::move(partialRequest), buffer);

    auto lengthString = buffer->toString(0, lineSep);
    auto trunkLength = std::stoul(lengthString, 0, 16);
    if (trunkLength + lineSep + 4 > buffer->len())
        return std::make_shared<TrunkedBodyParser>(std::move(partialRequest), buffer);

    // TODO Cannot push a buffer into another buffer. Try avoid useless copy!

    buffer->split(lineSep + 2); // Remove the line that indicates the length
    auto trunkData = buffer->split(trunkLength)->toString();
    auto trunkEnd = buffer->split(2);
    if (trunkEnd->toString() != "\r\n")
        throw HttpError(StatusCode::HTTP_BAD_REQUEST, "Bad trunk");


    partialRequest.body->push(trunkData.data(), trunkData.length());

    if (trunkLength == 0)
        return ParserHelper::buildFormParser(std::move(partialRequest), buffer)->process();

    return std::make_shared<TrunkedBodyParser>(std::move(partialRequest), buffer)->process();
}
