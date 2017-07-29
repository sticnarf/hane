#include "route.hpp"
#include "http/response.hpp"

namespace middleware
{
Route::Route(std::shared_ptr<Middleware> nextMiddleware)
        :Middleware(nextMiddleware) { }

void Route::call(const Request& req, std::shared_ptr<Response> resp)
{
    process(req, resp);
    if (nextMiddleware)
        nextMiddleware->call(req, resp);
}

void Route::process(const Request& req, std::shared_ptr<Response> resp)
{
    resp->setStatusCode(StatusCode::HTTP_OK);
    resp->headers.insert({"Content-Type", "text/html"});
    std::string resp_str = "<!DOCTYPE HTML>\n<title>Hello</title>\n<meta charset=\"UTF-8\">\n<h1>Hello world!</h1>\n";
    resp->body.insert(resp->body.end(), resp_str.begin(), resp_str.end());
}
}
