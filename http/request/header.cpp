#include "header.h"
#include "utils/string.h"

HeaderFieldEntry Header::getFieldEntry(std::string key)
{
    auto header = fields.find(toLowercase(key));

    if (header == fields.end())
    {
        return HeaderFieldEntry(false, HeaderField());
    }
    else
    {
        return HeaderFieldEntry(true, header->second);
    }
}

void Header::putField(const HeaderField& header)
{
    fields[toLowercase(header.name)] = header;
}

const std::string& HeaderField::getName() const
{
    return name;
}

bool HeaderFieldEntry::isValid() const
{
    return valid;
}

HeaderField HeaderFieldEntry::getHeaderField() const
{
    return header;
}

const std::string& HeaderField::getContent() const
{
    return content;
}

HeaderField::HeaderField(const std::string& name, const std::string& content)
        :name(name), content(content) { }

HeaderFieldEntry::HeaderFieldEntry(bool valid, const HeaderField& header)
        :valid(valid), header(header) { }
