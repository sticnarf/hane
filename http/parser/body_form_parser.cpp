//
// Created by Yilin Chen on 2017/7/24.
//

#include "body_form_parser.hpp"
#include "parser_helper.hpp"
#include "final_parser.hpp"

BodyFormParser::BodyFormParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr BodyFormParser::process()
{
    auto body = partialRequest.getBody();
    ParserHelper::parseUrlEncodedQueries(body->toString(), partialRequest);

    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
