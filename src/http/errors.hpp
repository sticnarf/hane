#ifndef HANE_ERRORS_HPP
#define HANE_ERRORS_HPP

#include <exception>
#include <string>
#include <utility>
#include "../constants.hpp"

class HttpError : std::exception {
private:
    StatusCode code;
    std::string reason;
public:
    HttpError(StatusCode code, std::string reason)
            : code(code), reason(std::move(reason)) {}

    StatusCode getCode() const {
        return code;
    }

    const std::string &getReason() const {
        return reason;
    }

};

#endif
