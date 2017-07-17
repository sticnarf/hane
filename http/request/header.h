#ifndef RACKCPP_HEADERS_H
#define RACKCPP_HEADERS_H
#include <map>
#include <string>

class HeaderField
{
private:
    std::string name;
    std::string content;
public:
    friend class Header;
    HeaderField() = default;
    HeaderField(const std::string& name, const std::string& content);
    const std::string& getName() const;
    const std::string& getContent() const;
};

class HeaderFieldEntry
{
private:
    bool valid;
    const HeaderField header;
public:
    bool isValid() const;
    HeaderFieldEntry(bool valid, const HeaderField& header);
    HeaderField getHeaderField() const;
};

class Header
{
    std::map<std::string, HeaderField> fields;

public:
    void putField(const HeaderField& header);
    HeaderFieldEntry getFieldEntry(std::string key);
};

#endif
