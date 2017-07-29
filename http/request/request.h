#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H
#include <string>
#include "constants.h"
#include "header.h"
#include "utils/buffer.h"
#include "form_data.h"

typedef BufferPtr Body;

class Request
{
public:
    HttpVersion getHttpVersion() const;
    HttpMethod getHttpMethod() const;
    const std::string& getTarget() const;
    const Header& getHeader() const;
    const Body& getBody() const;
    const std::map<std::string, FormData>& getQueries() const;

private:
    HttpVersion httpVersion;
    HttpMethod httpMethod;
    std::string target;
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
