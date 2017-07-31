#ifndef RACKCPP_REQUEST_HPP
#define RACKCPP_REQUEST_HPP

#include <string>
#include "constants.hpp"
#include "header.hpp"
#include "utils/buffer.hpp"
#include "form_data.hpp"

typedef BufferPtr Body;

class Request {
public:
    HttpVersion getHttpVersion() const;

    HttpMethod getHttpMethod() const;

    const std::string &getTarget() const;

    const Header &getHeader() const;

    const Body &getBody() const;

    const std::map<std::string, FormData> &getQueries() const;

private:
    HttpVersion httpVersion;
    HttpMethod httpMethod;
public:
    const std::string &getAbsPath() const;

private:
    std::string target;
    std::string absPath;
    std::map<std::string, FormData> queries;

    Header header;
    Body body;

    // Set all parser classes as friends
    friend class AbstractParser;

    friend class StartLineParser;

    friend class QueriesParser;

    friend class HeaderParser;

    friend class SizedBodyParser;

    friend class FinalParser;

    friend class ParserHelper;

    friend class MultipartFormParser;
};

#endif
