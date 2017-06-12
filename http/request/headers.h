#ifndef RACKCPP_HEADERS_H
#define RACKCPP_HEADERS_H
#include <map>
#include <string>
class Header
{
private:
    std::string key;
public:
    friend class Headers;
    const std::string& getKey() const;
};

class HeaderEntry {
private:
    bool valid;
    const Header* header;
public:
    bool isValid() const;
    HeaderEntry(bool valid, const Header* header);
    const Header* getHeader() const;
};

class Headers
{
    std::map<std::string, Header> headers;

public:
    void putHeader(const Header& header);
    HeaderEntry getHeaderEntry(std::string key);
};
#endif
