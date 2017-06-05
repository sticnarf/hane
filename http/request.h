#ifndef RACKCPP_REQUEST_H
#define RACKCPP_REQUEST_H

#include <string>
#include <vector>
#include <map>
#include <uv.h>
#include "http/method.h"
#include "http/header.h"

class HttpServer;

class Request
{
    friend class HttpServer;

    HttpServer* httpServer;
    uv_tcp_t* client;
    Method method;
    std::string requestTarget;
    std::string httpVersion;
    Header header;

private:
    std::map<std::string, std::string> queries;
    std::string body;

    class Parser
    {
        void parseRequestLine(Request& req);

        void parseUrlQueries(Request& req);

        void parseQueries(Request& req, const std::string& queryText);

        void parseHeaderFields(Request& req);

        void parseMessageBody(Request& req);

        void processBody(Request& req);

    public:
        enum class Stage
        {
            REQUEST_LINE,
            HEADER_FIELDS,
            MESSAGE_BODY,
            BODY_PROCESSING,
            PARSING_FINISHED
        } stage = Stage::REQUEST_LINE;
        std::string buf;
        size_t bufPos = 0;

        void push_buf(const char* buf, size_t len);

        void parse(Request& req);
    } parser;

public:
    Request(HttpServer* http_server, uv_tcp_t* client);

    void push_buf(const uv_buf_t* buf, ssize_t nread);

    void process();

    void infoLog(const std::string& msg);

    void errorLog(const std::string& msg);

    bool isInfoLogEnabled() const;

    bool isErrorLogEnabled() const;

    Method getMethod() const;

    const std::string& getRequestTarget() const;

    const std::string& getHttpVersion() const;

    const Header& getHeader() const;

    const std::map<std::string, std::string>& getQueries() const;

    const std::string& getBody() const;

    ~Request();
};

#endif
