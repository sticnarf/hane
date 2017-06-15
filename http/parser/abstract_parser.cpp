#include <stdexcept>
#include "abstract_parser.h"

AbstractParser::AbstractParser(Request&& req, Buffer& buffer)
    :partialRequest(req), buffer(buffer) { }

AbstractParser AbstractParser::process()
{
    throw std::logic_error("Invoke process() of AbstractParser");
}
