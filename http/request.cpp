#include <string>
#include <cctype>
#include <uv.h>
#include "request.h"
#include "http/http.h"

Request::Request(HttpServer* http_server, uv_tcp_t* client)
    :httpServer(http_server), client(client) { }

Request::~Request() { }

void Request::push_buf(const uv_buf_t* buf, ssize_t nread)
{
    parser.push_buf(buf->base, nread);
    parser.parse(*this);
}

void Request::process()
{
    auto new_req = new Request(httpServer, client);
    if (parser.bufPos<parser.buf.size())
    {
        new_req->parser.push_buf(parser.buf.data()+parser.bufPos, parser.buf.size()-parser.bufPos);
    }
    client->data = new_req;
    httpServer->process(*this);
}

Method Request::getMethod() const
{
    return method;
}

const std::map<std::string, std::string>& Request::getQueries() const
{
    return queries;
}

const std::string& Request::getRequestTarget() const
{
    return requestTarget;
}

const std::string& Request::getHttpVersion() const
{
    return httpVersion;
}

const Header& Request::getHeader() const
{
    return header;
}

const std::string& Request::getBody() const
{
    return body;
}

void Request::infoLog(const std::string& msg)
{
    httpServer->infoLog(msg);
}

void Request::errorLog(const std::string& msg)
{
    httpServer->errorLog(msg);
}

bool Request::isInfoLogEnabled() const
{
    return httpServer->isInfoLogEnabled();
}

bool Request::isErrorLogEnabled() const
{
    return httpServer->isErrorLogEnabled();
}

void Request::Parser::parseRequestLine(Request& req)
{
    size_t line_end = buf.find("\r\n", bufPos);
    if (line_end!=std::string::npos)
    {
        size_t sep = buf.find(' ', bufPos);
        req.method = parseMethod(buf.substr(bufPos, (sep++)-bufPos));
        size_t sep2 = buf.find(' ', sep);
        req.requestTarget = buf.substr(sep, (sep2++)-sep);
        parseUrlQueries(req);
        req.httpVersion = buf.substr(sep2, line_end-sep2);
        bufPos = line_end+2;
        stage = Stage::HEADER_FIELDS;
        parse(req);
    }
}

void Request::Parser::parseHeaderFields(Request& req)
{
    size_t line_end = buf.find("\r\n", bufPos);
    if (line_end==bufPos)
    {
        bufPos = line_end+2;
        stage = Stage::MESSAGE_BODY;
        parse(req);
    }
    else if (line_end!=std::string::npos)
    {
        size_t sep = buf.find(':', bufPos);
        std::string field_name = buf.substr(bufPos, (sep++)-bufPos);
        while (isspace(buf[sep])) sep++;
        size_t value_end = line_end-1;
        while (isspace(buf[value_end])) value_end--;
        std::string field_value = buf.substr(sep, value_end+1-sep);
        req.header.put(field_name, field_value);
        bufPos = line_end+2;
        parse(req);
    }
}

void Request::Parser::parseMessageBody(Request& req)
{
    switch (req.method)
    {
    case Method::HTTP_GET:stage = Stage::PARSING_FINISHED;
        break;
    default:auto transfer_encoding = req.header.getValue("Transfer-Encoding");
        if (transfer_encoding!=req.header.endIterator())
        {
            // TODO: Transfer-Encoding is not supported
        }
        else
        {
            int content_length = stoi(req.header.getValue("Content-Length")->second);
            if (buf.size()-bufPos>=content_length)
            {
                req.body += buf.substr(bufPos, content_length);
                bufPos += content_length;
                stage = Stage::BODY_PROCESSING;
            }
        }
        break;
    }
    parse(req);
}

void Request::Parser::push_buf(const char* buf, size_t len)
{
    this->buf.append(buf, len);
}

void Request::Parser::parse(Request& req)
{
    switch (stage)
    {
    case Stage::REQUEST_LINE:parseRequestLine(req);
        break;
    case Stage::HEADER_FIELDS:parseHeaderFields(req);
        break;
    case Stage::MESSAGE_BODY:parseMessageBody(req);
        break;
    case Stage::BODY_PROCESSING:processBody(req);
        break;
    case Stage::PARSING_FINISHED:req.process();
        break;
    }
}

void Request::Parser::parseUrlQueries(Request& req)
{
    const std::string& target = req.requestTarget;
    size_t begin = target.find('?');
    if (begin!=std::string::npos)
    {
        parseQueries(req, target.substr(begin+1));
    }
}

void Request::Parser::processBody(Request& req)
{
    std::string content_type = req.header.getValue("Content-Type")->second;
    if (content_type.find("x-www-form-urlencoded")!=std::string::npos)
    {
        parseQueries(req, req.body);
    }
    stage = Stage::PARSING_FINISHED;
    parse(req);
}

void Request::Parser::parseQueries(Request& req, const std::string& queryText)
{
    size_t begin = 0;
    while (isspace(queryText[begin]))
        begin++;
    for (;;)
    {
        size_t equal_sign = queryText.find('=', begin);
        if (equal_sign==std::string::npos)
            break;
        std::string key, val;
        for (size_t i = begin; i<equal_sign; i++)
        {
            if (queryText[i]=='%')
            {
                key.push_back((char) (std::stoi(queryText.substr(i+1, 2), 0, 16)));
                i += 2;
            }
            else
            {
                key.push_back(queryText[i]);
            }
        }
        size_t ampersand = queryText.find('&', equal_sign);
        size_t end = ampersand==std::string::npos ? queryText.size() : ampersand;
        for (size_t i = equal_sign+1; i<end; i++)
        {
            if (queryText[i]=='%')
            {
                val.push_back((char) (std::stoi(queryText.substr(i+1, 2), 0, 16)));
                i += 2;
            }
            else
            {
                val.push_back(queryText[i]);
            }
        }
        req.queries[key] = val;
        if (ampersand==std::string::npos)
            break;
        begin = ampersand+1;
    }
}
