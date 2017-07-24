#include "parser_helpers.h"

void ParserHelper::parseUrlEncodedQueries(const std::string& data, Request& req, size_t begin)
{
    bool eof = false;
    while (!eof)
    {
        size_t equal_sign = data.find('=', begin);
        if (equal_sign == std::string::npos)
            break;
        std::string key, val;
        for (size_t i = begin; i < equal_sign; i++)
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

        size_t ampersand = data.find('&', equal_sign);
        eof = ampersand == std::string::npos;
        size_t end = eof ? data.size() : ampersand;
        for (size_t i = equal_sign + 1; i < end; i++)
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
