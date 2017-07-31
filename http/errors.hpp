#ifndef RACKCPP_ERRORS_HPP
#define RACKCPP_ERRORS_HPP

#include <exception>
#include <string>
#include "constants.hpp"

class HttpError : std::exception {
private:
    StatusCode code;
    std::string reason;
public:
    HttpError(StatusCode code, const std::string &reason)
            : code(code), reason(reason) {}

    StatusCode getCode() const {
        return code;
    }

    const std::string &getReason() const {
        return reason;
    }

};

#endif
