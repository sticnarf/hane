#ifndef HANE_HEADER_PARSER_HPP
#define HANE_HEADER_PARSER_HPP

#include "./abstract_parser.hpp"

class HeaderParser : public AbstractParser {
    HeaderContentPtr parseField(const std::string &fieldName, const std::string &fieldContent);

public:
    HeaderParser(RequestPtr req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
