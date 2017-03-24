#include "header.h"
#include <cctype>

static std::string to_lowercase(const std::string &str) {
    std::string result;
    for (char c : str) {
        result.push_back(tolower(c));
    }
    return result;
}

size_t Header::size() {
    return data.size();
}

std::map<std::string, std::string>::const_iterator Header::get_value(const std::string &field_name) const {
    return data.find(to_lowercase(field_name));
}

std::map<std::string, std::string>::const_iterator Header::end_iterator() const {
    return data.end();
}

bool Header::contains(const std::string &field_name) {
    return (get_value(field_name) != end_iterator());
}

void Header::put(const std::string &field_name, const std::string &field_value) {
    data[to_lowercase(field_name)] = field_value;
}
