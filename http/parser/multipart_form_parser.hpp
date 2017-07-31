#ifndef RACKCPP_MULTIPART_FORM_PARSER_HPP
#define RACKCPP_MULTIPART_FORM_PARSER_HPP

#include "abstract_parser.hpp"

class MultipartFormParser : public AbstractParser {
public:
    MultipartFormParser(Request &&req, BufferPtr buffer);

    ParserPtr process();
};

#endif
