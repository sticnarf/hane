#include <stdexcept>
#include "abstract_parser.h"

AbstractParser::AbstractParser(Request&& req, Buffer& buffer)
    :partialRequest(req), buffer(buffer), finished(false) { }

std::shared_ptr<AbstractParser> AbstractParser::process()
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
