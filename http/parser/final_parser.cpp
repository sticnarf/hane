#include "final_parser.h"

FinalParser::FinalParser(Request&& req, Buffer& buffer)
        :AbstractParser(std::move(req), buffer) { }

std::shared_ptr<AbstractParser> FinalParser::process()
{
    this->finished = true;
    return std::shared_ptr<AbstractParser>(this);
}
