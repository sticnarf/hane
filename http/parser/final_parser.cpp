#include "final_parser.h"

FinalParser::FinalParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer)
{
    finished = true;
}