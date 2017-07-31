#include "final_parser.hpp"

#include <utility>

FinalParser::FinalParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), std::move(buffer)) {
    finished = true;
}

ParserPtr FinalParser::process() {
    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
