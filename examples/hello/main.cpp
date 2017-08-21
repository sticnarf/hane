#include <hane/hane.hpp>
#include <utility>
#include <regex>
#include "templates/hello.html.hpp"

class HelloMiddleware : public Middleware {
public:
    MiddlewarePtr call(Request &req, std::shared_ptr<Response> &resp) override {
        resp->headers.put("Content-Type", std::make_shared<HeaderContent>("text/html"));

        HelloHtml t;
        auto queries = req.getQueries();
        auto name = queries.find("name");
        if (name != queries.end()) {
            t.name = name->second.getData();
            Logger::getInstance().info("Get name: {}", t.name);
        }

        resp->body = t.render();

        return nullptr;
    }
};

class AssetsMiddleware : public Middleware {
public:
    MiddlewarePtr call(Request &req, std::shared_ptr<Response> &resp) override {
        auto sm = std::make_shared<StaticMiddleware>("." + req.getAbsPath());
        return sm->call(req, resp);
    }
};

class PrintBodyMiddleware : public Middleware {
public:
    MiddlewarePtr call(Request &req, std::shared_ptr<Response> &resp) override {
        for (auto &e : *req.getCookies()) {
            auto cookie = e.second;
            resp->body += fmt::format("{0} = {1}\r\n", cookie.name, cookie.value);
        }
//        resp->body = req.getBody()->toString();
        return nullptr;
    }
};

int main(int argc, char *argv[]) {
    Logger::getInstance().setLogPath("/var/log/hello.log");

    auto hello = std::make_shared<HelloMiddleware>();
    auto route = std::make_shared<RouteMiddleware>();
    auto assets = std::make_shared<AssetsMiddleware>();
    auto print = std::make_shared<PrintBodyMiddleware>();

    route->addRule(std::regex("/(hello){0,1}"), HttpMethod::METHOD_GET, hello);
    route->addRule(std::regex("/assets/.+"), HttpMethod::METHOD_GET, assets);
    route->addRule(std::regex("/print"), HttpMethod::METHOD_GET, print);

    HttpServer server(route, "0.0.0.0", 8089);
    server.start();

    return 0;
}
