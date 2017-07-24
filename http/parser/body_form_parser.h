#ifndef RACKCPP_BODY_FORM_PARSER_H
#define RACKCPP_BODY_FORM_PARSER_H

#include "abstract_parser.h"

class BodyFormParser : public AbstractParser
{

public:
    BodyFormParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
