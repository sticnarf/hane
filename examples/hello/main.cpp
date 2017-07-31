#include <http/http.hpp>
#include <utils/logger.hpp>
#include <middlewares/simple_middleware.hpp>
#include <middlewares/route.hpp>
#include <regex>

class HelloMiddleware : public SimpleMiddleware {
public:
    HelloMiddleware(std::shared_ptr<Middleware> next_middleware)
            : SimpleMiddleware(next_middleware) {}

    void process(const Request &req, std::shared_ptr<Response> resp) {
        resp->setStatusCode(StatusCode::HTTP_OK);
        resp->headers.insert({"Content-Type", "text/html"});

        std::string resp_str = "<!DOCTYPE HTML>\n<title>Hello</title>\n<meta charset=\"UTF-8\">\n<h1>";

        auto queries = req.getQueries();
        auto name = queries.find("name");
        if (name != queries.end()) {
            Logger::getInstance().info("Get name: {}", queries["name"].getData());
            resp_str += "Hello, " + name->second.getData() + "!";
        } else {
            resp_str += "Hello!";
        }

        resp_str += "</h1>\n";

        resp->body = resp_str;
    }
};

int main(int argc, char *argv[]) {
    Logger::getInstance().setLogPath("/var/log/hello.log");
    auto hello = std::make_shared<HelloMiddleware>(nullptr);
    auto route = std::make_shared<RouteMiddleware>();
    route->addRule(std::regex("\\/(hello){0,1}"), hello);
    HttpServer server(route, "0.0.0.0", 8089);
    server.start();
    return 0;
}
