//
// Created by Yilin Chen on 2017/8/20.
//

#include "cookies.hpp"
#include "../parser/parser_helper.hpp"
#include <stdexcept>
#include <sstream>
#include <cstring>

std::string Cookie::toCookieString() {
    if (!CookieHelper::validateName(name))
        throw std::invalid_argument("Cookie name is not a valid token");

    if (!CookieHelper::validateOctet(value))
        throw std::invalid_argument("Cookie octet is not valid");

    std::stringstream ss;
    ss << name << "=" << value;

    if (expires > 0) {
        ss << "; Expires=" << CookieHelper::toRfc1123(expires);
    }

    if (maxAge > 0) {
        ss << "; Max-Age=" << maxAge;
    }

    if (!domain.empty()) {
        ss << "; Domain=" << domain;
    }

    if (!path.empty()) {
        ss << "; Path=" << path;
    }

    if (secure) {
        ss << "; Secure";
    }

    if (httpOnly) {
        ss << "; HttpOnly";
    }


    return ss.str();
}

Cookie::Cookie(const std::string &name, const std::string &value) : name(name), value(value) {}

bool CookieHelper::validateName(std::string name) {
    return ParserHelper::validateToken(name);
}

bool CookieHelper::validateOctet(std::string octet) {
    for (auto c : octet) {
        if (c < 0x21 || c == 0x22 || c == 0x2C || c == 0x3B || c == 0x5C || c > 0x7E)
            return false;
    }
    return true;
}

static const char *DAY_NAMES[] =
        {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
static const char *MONTH_NAMES[] =
        {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
         "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

/**
 * Attention: This function is not thread-safe!
 * @param t
 * @return the RFC1123-date string
 */
std::string CookieHelper::toRfc1123(time_t t) {
    const int RFC1123_TIME_LEN = 29;

    auto *buf = new char[RFC1123_TIME_LEN + 1];
    std::time(&t);
    auto tm = std::gmtime(&t);

    strftime(buf, RFC1123_TIME_LEN + 1, "---, %d --- %Y %H:%M:%S GMT", tm);
    memcpy(buf, DAY_NAMES[tm->tm_wday], 3);
    memcpy(buf + 8, MONTH_NAMES[tm->tm_mon], 3);

    std::string result(buf);
    delete[] buf;

    return result;
}
