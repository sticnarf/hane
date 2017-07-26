#ifndef RACKCPP_CONTENT_TYPE_H
#define RACKCPP_CONTENT_TYPE_H

#include "http/request/header.h"
#include <string>
#include <map>
#include "http/parser/parser_helpers.h"

class ContentType: HeaderField
{
public:
    ContentType(const std::string& content);
private:
    std::string mediaType;
    FieldParameters parameters;
};

#endif
