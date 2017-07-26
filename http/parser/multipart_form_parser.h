#ifndef RACKCPP_MULTIPART_FORM_PARSER_H
#define RACKCPP_MULTIPART_FORM_PARSER_H

#include "abstract_parser.h"

class MultipartFormParser: public AbstractParser
{
public:
    MultipartFormParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
