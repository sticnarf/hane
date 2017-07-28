#include "parser_helper.h"
#include <cctype>
#include <cstring>

void ParserHelper::parseUrlEncodedQueries(const std::string& data, Request& req, size_t begin)
{
    bool eof = false;
    while (!eof)
    {
        size_t equalSign = data.find('=', begin);
        if (equalSign == std::string::npos)
            break;
        std::string key, val;
        for (size_t i = begin; i < equalSign; i++)
        {
            if (data[i] == '%')
            {
                key += (char) std::stoi(data.substr(i + 1, 2), 0, 16);
                i += 2;
            }
            else
            {
                key += data[i];
            }
        }

        size_t ampersand = data.find('&', equalSign);
        eof = ampersand == std::string::npos;
        size_t end = eof ? data.size() : ampersand;
        for (size_t i = equalSign + 1; i < end; i++)
        {
            if (data[i] == '%')
            {
                val += (char) std::stoi(data.substr(i + 1, 2), 0, 16);
                i += 2;
            }
            else
            {
                val += data[i];
            }
        }

        req.queries.insert({key, val});
    }
}

void ParserHelper::parseParameters(const std::string& data, FieldParameters& parameters, size_t begin)
{
    bool eof = false;
    while (!eof)
    {
        size_t equalSign = data.find('=', begin);
        if (equalSign == std::string::npos)
            break;

        std::string key, val;
        key = data.substr(begin, equalSign - 1);
        StringUtils::trim(key);
        if(!validateToken(key))
            throw std::invalid_argument("invalid header parameter");

        size_t valueBegin = equalSign + 1;
        while (isspace(data[valueBegin]))
            valueBegin++;

        if (data[valueBegin] == '"' && valueBegin < (data.length() - 1))
        {
            // Quoted string
            size_t rightQuote = data.find('"', valueBegin + 1);
            if (rightQuote == std::string::npos)
                throw std::invalid_argument("invalid header value");

            val = data.substr(valueBegin, rightQuote - valueBegin - 1);
        } else {
            // token
            size_t semicolon = data.find(';', equalSign);
            eof = semicolon == std::string::npos;
            size_t end = eof ? data.size() : semicolon;

            val = data.substr(equalSign + 1, end);
            StringUtils::trim(val);
            if(!validateToken(val))
                throw std::invalid_argument("invalid header value");
        }

        parameters.put(key, val);
    }
}

bool ParserHelper::validateToken(const std::string& name)
{
    static const char* list = "!#$%&'*+-.^_`|~";
    static const size_t listLen = strlen(list);

    if (name.length() == 0)
        return false;

    for (char c : name)
    {
        if (isalnum(c))
            continue;

        bool ok = false;
        for (int i = 0; i < listLen; i++)
        {
            if (c == list[i])
            {
                ok = true;
                break;
            }
        }

        if (!ok)
            return false;
    }

    return true;
}

bool ParserHelper::validateHeaderFieldContent(const std::string& content)
{
    for (char c : content)
    {
        if (!isprint(c))
            return false;
    }

    return true;
}