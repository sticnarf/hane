#include "headers.h"
#include "utils/string.h"

HeaderEntry Headers::getHeaderEntry(std::string key)
{
    auto header = headers.find(toLowercase(key));
    if (header == headers.end())
    {
        return HeaderEntry(false, nullptr);
    }
    else
    {
        return HeaderEntry(true, &header->second);
    }
}

void Headers::putHeader(const Header& header)
{
    headers[toLowercase(header.key)] = header;
}

const std::string& Header::getKey() const
{
    return key;
}

bool HeaderEntry::isValid() const
{
    return valid;
}

const Header* HeaderEntry::getHeader() const
{
    return header;
}

HeaderEntry::HeaderEntry(bool valid, const Header* header)
        :valid(valid), header(header) { }
