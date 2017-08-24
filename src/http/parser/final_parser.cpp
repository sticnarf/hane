#include "final_parser.hpp"

#include <utility>

FinalParser::FinalParser(RequestPtr req, BufferPtr buffer)
        : AbstractParser(req, buffer) {
    finished = true;
}

ParserPtr FinalParser::process() {
    return shared_from_this();
}
