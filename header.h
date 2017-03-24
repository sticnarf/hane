#ifndef RACKCPP_HEADER_H
#define RACKCPP_HEADER_H

#include <map>
#include <string>

class Header {
    friend class Request;

    std::map<std::string, std::string> data;

    void put(const std::string &field_name, const std::string &field_value);

public:
    size_t size();

    std::map<std::string, std::string>::const_iterator end_iterator() const;

    std::map<std::string, std::string>::const_iterator get_value(const std::string &field_name) const;

    bool contains(const std::string &field_name);
};

#endif
