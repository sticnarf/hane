//
// Created by Yilin Chen on 2017/7/24.
//

#include "body_form_parser.h"
#include "parser_helper.h"
#include "final_parser.h"

BodyFormParser::BodyFormParser(Request&& req, BufferPtr buffer)
        :AbstractParser(std::move(req), buffer) { }

ParserPtr BodyFormParser::process()
{
    auto body = partialRequest.getBody();
    ParserHelper::parseUrlEncodedQueries(body->toString(), partialRequest);

    return std::make_shared<FinalParser>(std::move(partialRequest), buffer);
}
