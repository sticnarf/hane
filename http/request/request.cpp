#include "request.h"

HttpVersion Request::getHttpVersion() const
{
    return httpVersion;
}

HttpMethod Request::getHttpMethod() const
{
    return httpMethod;
}

const std::string& Request::getTarget() const
{
    return target;
}

const Header& Request::getHeader() const
{
    return header;
}

const Body& Request::getBody() const
{
    return body;
}

const std::map<std::string, FormData>& Request::getQueries() const
{
    return queries;
}
