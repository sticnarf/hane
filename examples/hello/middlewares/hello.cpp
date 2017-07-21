#include <rackcpp.h>
#include "hello.h"

namespace middleware
{
Hello::Hello(std::shared_ptr<Middleware> next_middleware)
        :Middleware(next_middleware) { }

void Hello::call(const Request& req, std::shared_ptr<Response> resp)
{
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}

void Hello::process(const Request& req, std::shared_ptr<Response> resp)
{
    resp->setStatusCode(StatusCode::HTTP_OK);
    resp->headers.insert({"Content-Type", "text/html"});
    auto queries = req.getQueries();
    Logger::getInstance().info("Get name:" + queries["name"] + "\n");
    std::string resp_str =
            "<!DOCTYPE HTML>\n<title>Hello</title>\n<meta charset=\"UTF-8\">\n<h1>Hello, " + queries["name"] +
                    "!</h1>\n";
    resp->body += resp_str;
}

}
