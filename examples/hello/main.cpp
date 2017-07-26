#include <rackcpp.h>
#include "middlewares/hello.h"

int main(int argc, char* argv[])
{
    Logger::getInstance().setLogPath("/var/log/hello.log");
    auto hello = std::make_shared<middleware::Hello>(nullptr);
    HttpServer server(hello, "0.0.0.0", 8089);
    server.start();
    return 0;
}
