#include "body_parser.h"

RawBodyParser::RawBodyParser(Request&& req, Buffer& buffer)
        :AbstractParser(std::move(req), buffer) { }

std::unique_ptr<AbstractParser> RawBodyParser::process()
{
    // TODO
    return AbstractParser::process();
}
