#include "content_type.h"

const std::string ContentType::getLowercasedMediaType() const
{
    return StringUtils::toLowercase(mainContent);
}