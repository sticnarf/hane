#include "content_type.hpp"

const std::string ContentType::getLowercasedMediaType() const {
    return StringUtils::toLowercase(mainContent);
}