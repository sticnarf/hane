#include "final_parser.hpp"

FinalParser::FinalParser(Request &&req, BufferPtr buffer)
        : AbstractParser(std::move(req), buffer) {
    finished = true;
}

ParserPtr FinalParser::process() {
    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
