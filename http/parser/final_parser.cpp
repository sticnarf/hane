#include "final_parser.h"

FinalParser::FinalParser(Request&& req, std::shared_ptr<Buffer> buffer)
        :AbstractParser(std::move(req), buffer)
{
    finished = true;
}