//
// Created by Yilin Chen on 2017/8/26.
//

#include "bad_request.hpp"

bool BadRequest::isBad() {
    return true;
}

BadRequest::BadRequest(const HttpError &error) : error(error) {}

HttpError BadRequest::getError() {
    return error;
}
