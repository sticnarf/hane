#ifndef HANE_CONTENT_TYPE_HPP
#define HANE_CONTENT_TYPE_HPP

#include "../header.hpp"
#include <string>
#include <map>

class ContentType : public HeaderContentWithParameters {
public:
    const std::string getLowercasedMediaType() const;
};

typedef std::shared_ptr<ContentType> ContentTypePtr;

#endif
