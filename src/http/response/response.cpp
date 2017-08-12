#include"response.hpp"

void Response::setStatusCode(StatusCode statusCode) {

    this->statusCode = statusCode;
}

Response::Response(HttpVersion version)
        : httpVersion(version) {
    setStatusCode(StatusCode::HTTP_OK);
}

Response::~Response() = default;


bool Response::isChunked() const {
    auto transferEncodingEntry = headers.get("Transfer-Encoding");
    return (transferEncodingEntry.isValid() && transferEncodingEntry.getValue()->getContent() == "chunked");
}