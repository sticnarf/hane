#ifndef RACKCPP_HEADERS_H
#define RACKCPP_HEADERS_H
#include <map>
#include <string>
#include <memory>
#include "utils/case_insensitive_map.hpp"

class HeaderContent
{
protected:
    std::string content;
public:
    const std::string& getContent() const
    {
        return content;
    }

    HeaderContent() = default;

    HeaderContent(const std::string& content)
            :content(content) { }

    virtual ~HeaderContent() = default;

    friend class ParserHelper;
};

typedef std::shared_ptr<HeaderContent> HeaderContentPtr;

class Header: public CaseInsensitiveMap<HeaderContentPtr>
{

};

typedef CaseInsensitiveMap<std::string> FieldParameters;

class HeaderContentWithParameters;

typedef std::shared_ptr<HeaderContentWithParameters> HeaderContentWithParametersPtr;

class HeaderContentWithParameters: public HeaderContent
{
protected:
    // Case insensitive!
    std::string mainContent;

    FieldParameters parameters;
public:
    const std::string& getMainContent() const
    {
        return mainContent;
    }

    const FieldParameters& getParameters() const
    {
        return parameters;
    }

    virtual ~HeaderContentWithParameters() = default;

    friend class ParserHelper;
};

#endif
