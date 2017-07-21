#include "queries_parser.h"
#include "header_parser.h"

QueriesParser::QueriesParser(Request&& req, std::shared_ptr<Buffer> buffer)
        :AbstractParser(std::move(req), buffer) { }

std::shared_ptr<AbstractParser> QueriesParser::process()
{
    const std::string& target = partialRequest.target;
    size_t begin = target.find_last_of('?');
    while (begin != std::string::npos)
    {
        size_t equal_sign = target.find('=', begin);
        if (equal_sign == std::string::npos)
            break;
        std::string key, val;
        for (size_t i = begin + 1; i < equal_sign; i++)
        {
            if (target[i] == '%')
            {
                key += (char) std::stoi(target.substr(i + 1, 2), 0, 16);
            }
            else
            {
                key += target[i];
            }
        }
        size_t ampersand = target.find('&', equal_sign);
        size_t end = ampersand == std::string::npos ? target.size() : ampersand;
        for (size_t i = equal_sign + 1; i < end; i++)
        {
            if (target[i] == '%')
            {
                val += (char) std::stoi(target.substr(i + 1, 2), 0, 16);
            }
            else
            {
                val += target[i];
            }
        }
        partialRequest.queries.insert({key, val});
        begin = ampersand;
    }

    return HeaderParser(std::move(partialRequest), buffer).process();
}