#include "content_type.h"

const std::string& ContentType::getMediaType() const
{
    return mediaType;
}

const FieldParameters& ContentType::getParameters() const
{
    return parameters;
}

const std::string ContentType::getLowercasedMediaType() const
{
    return StringUtils::toLowercase(mediaType);
}