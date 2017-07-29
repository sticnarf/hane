#ifndef RACKCPP_HEADER_PARSER_H
#define RACKCPP_HEADER_PARSER_H

#include "abstract_parser.hpp"

class HeaderParser : public AbstractParser
{
    ParserPtr buildBodyParser();

    HeaderContentPtr parseField(const std::string& fieldName, const std::string& fieldContent);

public:
    HeaderParser(Request&& req, BufferPtr buffer);
    ParserPtr process();
};

#endif
