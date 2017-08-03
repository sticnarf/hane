#ifndef HANE_HEADER_PARSER_HPP
#define HANE_HEADER_PARSER_HPP

#include "./abstract_parser.hpp"

class HeaderParser : public AbstractParser {
    ParserPtr buildBodyParser();

    HeaderContentPtr parseField(const std::string &fieldName, const std::string &fieldContent);

public:
    HeaderParser(Request &&req, BufferPtr buffer);

    ParserPtr process() override;
};

#endif
