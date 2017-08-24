#include <stdexcept>
#include <utility>
#include "abstract_parser.hpp"

AbstractParser::AbstractParser(RequestPtr req, BufferPtr buffer)
        : partialRequest(req), buffer(buffer), finished(false) {}

ParserPtr AbstractParser::process() {
    throw std::logic_error("Invoke process() of AbstractParser");
}

bool AbstractParser::isFinished() const {
    return finished;
}

RequestPtr AbstractParser::getRequest() const {
    return partialRequest;
}
