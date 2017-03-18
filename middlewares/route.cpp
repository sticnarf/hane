#include "../middleware.h"
#include "route.h"
#include <iostream>

namespace middleware {
    Route::Route(Middleware *next_middleware) {
        this->next_middleware = next_middleware;
    }

    void Route::call(const Request &req, Response &resp) {
        process(req, resp);
        if (next_middleware)
            next_middleware->call(req, resp);
    }

    void Route::process(const Request &req, Response &resp) {
        resp.set_status_code(Response::StatusCode::HTTP_OK);
        resp.headers.insert({"Content-Type", "text/html"});
        std::string resp_str = "<!DOCTYPE HTML>\n<title>Hello</title>\n<meta charset=\"UTF-8\">\n<h1>Hello world!</h1>\n";
        resp.body.insert(resp.body.end(), resp_str.begin(), resp_str.end());
    }

    Route::~Route() {
        if (next_middleware)
            delete next_middleware;
    }
}
