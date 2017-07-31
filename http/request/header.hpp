#ifndef HANE_HEADERS_HPP
#define HANE_HEADERS_HPP

#include <map>
#include <string>
#include <memory>
#include <utility>
#include "utils/case_insensitive_map.hpp"

class HeaderContent {
protected:
    std::string content;
public:
    const std::string &getContent() const {
        return content;
    }

    HeaderContent() = default;

    HeaderContent(std::string content)
            : content(std::move(content)) {}

    virtual ~HeaderContent() = default;

    friend class ParserHelper;
};

typedef std::shared_ptr<HeaderContent> HeaderContentPtr;

class Header : public CaseInsensitiveMap<HeaderContentPtr> {

};

typedef CaseInsensitiveMap<std::string> FieldParameters;

class HeaderContentWithParameters;

typedef std::shared_ptr<HeaderContentWithParameters> HeaderContentWithParametersPtr;

class HeaderContentWithParameters : public HeaderContent {
protected:
    // Case insensitive!
    std::string mainContent;

    FieldParameters parameters;
public:
    const std::string &getMainContent() const {
        return mainContent;
    }

    const FieldParameters &getParameters() const {
        return parameters;
    }

    virtual ~HeaderContentWithParameters() override = default;

    friend class ParserHelper;
};

#endif
