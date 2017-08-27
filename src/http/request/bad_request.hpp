#ifndef HANE_BAD_REQUEST_HPP
#define HANE_BAD_REQUEST_HPP

#include "../errors.hpp"
#include "request.hpp"

class BadRequest : public Request {
    HttpError error;
public:
    explicit BadRequest(const HttpError &error);

    bool isBad() override;

    HttpError getError();
};

#endif
