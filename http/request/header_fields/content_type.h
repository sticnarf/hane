#ifndef RACKCPP_CONTENT_TYPE_H
#define RACKCPP_CONTENT_TYPE_H

#include "http/request/header.h"
#include <string>
#include <map>

class ContentType: public HeaderContent
{
private:
    // Case insensitive!
    std::string mediaType;

    FieldParameters parameters;
public:
    const std::string& getMediaType() const;
    const std::string getLowercasedMediaType() const;
    const FieldParameters& getParameters() const;

    friend HeaderContentPtr parseContentType(const std::string& fieldContent);
};

#endif
