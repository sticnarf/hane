#ifndef RACKCPP_CONTENT_TYPE_H
#define RACKCPP_CONTENT_TYPE_H

#include "http/request/header.hpp"
#include <string>
#include <map>

class ContentType: public HeaderContentWithParameters
{
public:
    const std::string getLowercasedMediaType() const;
};

typedef std::shared_ptr<ContentType> ContentTypePtr;

#endif
