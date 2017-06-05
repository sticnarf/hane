#include "hello.h"
#include "http/request.h"
#include "http/response.h"

namespace middleware
{
Hello::Hello(Middleware *next_middleware)
{
    this->nextMiddleware = next_middleware;
}

void Hello::call(const Request &req, Response &resp)
{
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}

void Hello::process(const Request &req, Response &resp)
{
    resp.setStatusCode(Response::StatusCode::HTTP_OK);
    resp.headers.insert({"Content-Type", "text/html"});
    auto queries = req.getQueries();
    std::string resp_str =
        "<!DOCTYPE HTML>\n<title>Hello</title>\n<meta charset=\"UTF-8\">\n<h1>Hello, " + queries["name"] +
            "!</h1>\n";
    resp.body.insert(resp.body.end(), resp_str.begin(), resp_str.end());
}

Hello::~Hello()
{
    if (nextMiddleware)
        delete nextMiddleware;
}
}
