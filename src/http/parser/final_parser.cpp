#include "final_parser.hpp"

#include <utility>

FinalParser::FinalParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), buffer) {
    finished = true;
}

ParserPtr FinalParser::process() {
    return shared_from_this();
}
