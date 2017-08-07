#include <hane/hane.hpp>
#include <utility>
#include <regex>
#include "templates/hello.html.hpp"

class HelloMiddleware : public Middleware {
public:
    void call(const Request &req, std::shared_ptr<Response> resp) override {
        resp->headers.insert({"Content-Type", "text/html"});

        HelloHtml t;
        auto queries = req.getQueries();
        auto name = queries.find("name");
        if (name != queries.end()) {
            t.name = name->second.getData();
            Logger::getInstance().info("Get name: {}", t.name);
        }

        resp->body = t.render();
    }
};

class AssetsMiddleware : public Middleware {
public:
    void call(const Request &req, std::shared_ptr<Response> resp) override {
        StaticMiddleware sm("." + req.getAbsPath());
        sm.call(req, resp);
    }
};

#include <iostream>

class PrintBodyMiddleware : public Middleware {
public:
    void call(const Request &req, std::shared_ptr<Response> resp) override {
        resp->body = req.getBody()->toString();
        std::cout << resp->body << std::endl;
    }
};

int main(int argc, char *argv[]) {
    Logger::getInstance().setLogPath("/var/log/hello.log");

    auto hello = std::make_shared<HelloMiddleware>();
    auto route = std::make_shared<RouteMiddleware>();
    auto assets = std::make_shared<AssetsMiddleware>();
    auto print = std::make_shared<PrintBodyMiddleware>();

    route->addRule(std::regex("\\/(hello){0,1}"), hello);
    route->addRule(std::regex("\\/assets/.+"), assets);
    route->addRule(std::regex("\\/print"), print);

    HttpServer server(route, "0.0.0.0", 8089);
    server.start();

    return 0;
}
