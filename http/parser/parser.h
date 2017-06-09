#ifndef RACKCPP_PARSER_H
#define RACKCPP_PARSER_H

class Parser
{
private:

public:
    enum class Stage
    {
        REQUEST_LINE,
        HEADER_FIELDS,
        MESSAGE_BODY,
        BODY_PROCESSING,
        PARSING_FINISHED
    } stage = Stage::REQUEST_LINE;

};

#endif
