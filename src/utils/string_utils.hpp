#ifndef HANE_STRING_UTILS_HPP
#define HANE_STRING_UTILS_HPP

#include <string>

class StringUtils {
public:
    static std::string toLowercase(const std::string &str);

    static std::string processQuotedPair(const std::string &str);

    // See: https://stackoverflow.com/questions/216823/whats-the-best-way-to-trim-stdstring
    static void ltrim(std::string &s);

    static void rtrim(std::string &s);

    static void trim(std::string &s);

    static std::string ltrim_copy(std::string s);

    static std::string rtrim_copy(std::string s);

    static std::string trim_copy(std::string s);
};

#endif
