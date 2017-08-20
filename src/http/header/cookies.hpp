#ifndef HANE_COOKIES_HPP
#define HANE_COOKIES_HPP

#include <string>
#include <chrono>
#include <ctime>
#include <memory>
#include <map>
#include "header.hpp"

struct Cookie {
    std::string name;
    std::string value;
    time_t expires;
    int maxAge{0};
    std::string domain;
    std::string path;
    bool secure{false};
    bool httpOnly{false};

    std::string toCookieString();

    Cookie() = default;

    Cookie(const std::string &name, const std::string &value);
};

struct CookieHelper {
    static bool validateName(std::string name);

    static bool validateOctet(std::string octet);

    static std::string toRfc1123(time_t time);
};

class Cookies : public std::map<std::string, Cookie>, public HeaderContent {
};

typedef std::shared_ptr<Cookies> CookiesPtr;

#endif
