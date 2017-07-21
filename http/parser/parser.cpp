#include "parser.h"
#include "start_line_parser.h"

void Parser::pushBuf(const char* buf, size_t len)
{
    buffer->push(buf, len);
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
    currentParser = currentParser->process();

    if (currentParser->isFinished())
    {
        completeRequests.push(currentParser->getRequest());
        currentParser = std::shared_ptr<AbstractParser>(new StartLineParser(Request(), buffer));
    }
}

Parser::Parser()
{
    buffer = std::shared_ptr<Buffer>(new Buffer);
    currentParser = std::shared_ptr<AbstractParser>(new StartLineParser(Request(), buffer));
}
