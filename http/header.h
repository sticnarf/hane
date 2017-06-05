#ifndef RACKCPP_HEADER_H
#define RACKCPP_HEADER_H

#include <map>
#include <string>

class Header {
    friend class Request;

    std::map<std::string, std::string> data;

    void put(const std::string &fieldName, const std::string &fieldValue);

public:
    size_t size();

    std::map<std::string, std::string>::const_iterator endIterator() const;

    std::map<std::string, std::string>::const_iterator getValue(const std::string &fieldName) const;

    bool contains(const std::string &fieldName);
};

#endif
