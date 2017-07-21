#include <rackcpp.h>
#include "middlewares/hello.h"

int main(int argc, char* argv[])
{
    Logger::getInstance().setInfoPath("/tmp/test.log");
    std::unique_ptr<Middleware> hello = std::unique_ptr<Middleware>(new middleware::Hello(nullptr));
    HttpServer server(std::move(hello), "0.0.0.0", 8089);
    server.start();
    return 0;
}
