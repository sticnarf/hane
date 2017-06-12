#include "parser.h"

void Parser::pushBuf(const char* buf, size_t len)
{
    buffer.push(buf, len);
    process();
}

Request Parser::yieldRequest()
{
    return Request(nullptr, nullptr);
}
void Parser::process()
{

}
