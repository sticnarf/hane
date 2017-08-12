#include"response.hpp"

void Response::setStatusCode(StatusCode statusCode) {

    this->statusCode = statusCode;
}

Response::Response(HttpVersion version)
        : httpVersion(version) {
    setStatusCode(StatusCode::HTTP_OK);
}

Response::~Response() = default;


bool Response::isChunked() {
    return false;
}