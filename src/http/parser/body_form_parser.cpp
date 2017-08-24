//
// Created by Yilin Chen on 2017/7/24.
//

#include "body_form_parser.hpp"

#include <utility>
#include "parser_helper.hpp"
#include "final_parser.hpp"

BodyFormParser::BodyFormParser(RequestPtr req, BufferPtr buffer)
        : AbstractParser(req, buffer) {}

ParserPtr BodyFormParser::process() {
    auto body = partialRequest->getBody();
    ParserHelper::parseUrlEncodedQueries(body->toString(), partialRequest);

    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
