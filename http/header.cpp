#include "http/header.h"
#include <cctype>

static std::string toLowercase(const std::string &str) {
    std::string result;
    for (char c : str) {
        result.push_back(tolower(c));
    }
    return result;
}

size_t Header::size() {
    return data.size();
}

std::map<std::string, std::string>::const_iterator Header::getValue(const std::string &fieldName) const {
    return data.find(toLowercase(fieldName));
}

std::map<std::string, std::string>::const_iterator Header::endIterator() const {
    return data.end();
}

bool Header::contains(const std::string &fieldName) {
    return (getValue(fieldName) != endIterator());
}

void Header::put(const std::string &fieldName, const std::string &fieldValue) {
    data[toLowercase(fieldName)] = fieldValue;
}
