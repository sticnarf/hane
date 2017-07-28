#ifndef RACKCPP_HEADERS_H
#define RACKCPP_HEADERS_H
#include <map>
#include <string>
#include <memory>
#include "utils/case_insensitive_map.h"

class HeaderContent
{
private:
    std::string content;
public:
    const std::string& getContent() const
    {
        return content;
    }

    HeaderContent() = default;

    HeaderContent(const std::string& content)
            :content(content) { }
};

typedef std::shared_ptr<HeaderContent> HeaderContentPtr;

class Header: public CaseInsensitiveMap<HeaderContentPtr>
{

};

typedef CaseInsensitiveMap<std::string> FieldParameters;

#endif
