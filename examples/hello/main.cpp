#include <hane/hane.hpp>
#include <utility>
#include <regex>
#include "templates/hello.html.hpp"

class HelloMiddleware : public SimpleMiddleware {
public:
    explicit HelloMiddleware(std::shared_ptr<Middleware> next_middleware)
            : SimpleMiddleware(std::move(next_middleware)) {}

    void process(const Request &req, std::shared_ptr<Response> resp) override {
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

int main(int argc, char *argv[]) {
    Logger::getInstance().setLogPath("/var/log/hello.log");

    auto hello = std::make_shared<HelloMiddleware>(nullptr);
    auto route = std::make_shared<RouteMiddleware>();
    auto assets = std::make_shared<AssetsMiddleware>();

    route->addRule(std::regex("\\/(hello){0,1}"), hello);
    route->addRule(std::regex("\\/assets/.+"), assets);

    HttpServer server(route, "0.0.0.0", 8089);
    server.start();

    return 0;
}
