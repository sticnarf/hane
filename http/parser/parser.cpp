#include "parser.h"

void Parser::pushBuf(const char* buf, size_t len)
{
    buffer.push(buf, len);
    process();
}

Request Parser::yieldRequest()
{
    Request req = completeRequests.front();
    completeRequests.pop();
    return req; // Possible optimization using std::move?
}

bool Parser::hasCompleteRequest()
{
    return !completeRequests.empty();
}

void Parser::process()
{

}
