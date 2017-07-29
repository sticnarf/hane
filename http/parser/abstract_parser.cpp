#include <stdexcept>
#include "abstract_parser.hpp"

AbstractParser::AbstractParser(Request&& req, BufferPtr buffer)
    :partialRequest(req), buffer(buffer), finished(false) { }

ParserPtr AbstractParser::process()
{
    throw std::logic_error("Invoke process() of AbstractParser");
}

bool AbstractParser::isFinished() const
{
    return finished;
}

Request AbstractParser::getRequest() const
{
    return partialRequest;
}
