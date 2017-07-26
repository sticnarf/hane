#ifndef RACKCPP_HEADERS_H
#define RACKCPP_HEADERS_H
#include <map>
#include <string>
#include "utils/case_insensitive_map.h"

class HeaderField
{
private:
    std::string content;
public:
    const std::string& getContent() const
    {
        return content;
    }

    HeaderField() = default;

    HeaderField(const std::string& content)
            :content(content) { }
};

class Header: public CaseInsensitiveMap<HeaderField>
{

};

#endif
