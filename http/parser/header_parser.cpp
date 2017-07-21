#include "header_parser.h"
#include "sized_body_parser.h"
#include <cctype>
#include <cstring>

static bool validateName(const std::string& name)
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

static bool validateContent(const std::string& content)
{
    for (char c : content)
    {
        if (!isprint(c))
            return false;
    }

    return true;
}

HeaderParser::HeaderParser(Request&& req, std::shared_ptr<Buffer> buffer)
        :AbstractParser(std::move(req), buffer) { }

std::shared_ptr<AbstractParser> HeaderParser::process()
{
    size_t lineSep = buffer->find("\r\n", 2);
    if (lineSep >= buffer->len())
        return std::shared_ptr<AbstractParser>(this);

    auto fieldBuf = buffer->split(lineSep + 2);
    if (lineSep == 0)
    {
        return buildBodyParser()->process();
    }

    std::string headerField = fieldBuf->toString(0, lineSep);

    size_t colon = headerField.find(':');
    std::string fieldName = headerField.substr(0, colon);
    if (!validateName(fieldName))
        throw std::invalid_argument("Bad field name");

    size_t firstNotSpace = colon + 1;
    while (isspace(headerField[firstNotSpace]))
        firstNotSpace++;

    size_t lastNotSpace = headerField.length() - 1;
    while (isspace(headerField[lastNotSpace]))
        lastNotSpace--;

    std::string fieldContent = headerField.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1);
    if (!validateContent(fieldContent))
        throw std::invalid_argument("Bad field content");

    partialRequest.header.putField(HeaderField(fieldName, fieldContent));

    return this->process();
}

std::shared_ptr<AbstractParser> HeaderParser::buildBodyParser()
{
    // TODO Build different BodyParser according to Transfer-Encoding
    return std::make_shared<SizedBodyParser>(std::move(partialRequest), buffer);
}
