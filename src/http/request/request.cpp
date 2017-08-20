#include "request.hpp"

HttpVersion Request::getHttpVersion() const {
    return httpVersion;
}

HttpMethod Request::getHttpMethod() const {
    return httpMethod;
}

const std::string &Request::getTarget() const {
    return target;
}

const Header &Request::getHeader() const {
    return header;
}

const Body &Request::getBody() const {
    return body;
}

const std::map<std::string, FormData> &Request::getQueries() const {
    return queries;
}

const std::string &Request::getAbsPath() const {
    return absPath;
}

Request::Request() {
    body = std::make_shared<Buffer>();
}

CookiesPtr Request::getCookies() const {
    auto entry = header.get("Cookie");
    if (entry.isValid()) {
        return std::dynamic_pointer_cast<Cookies>(entry.getValue());
    } else {
        return std::make_shared<Cookies>();
    }
}
