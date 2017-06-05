#include <rackcpp.h>
#include "middlewares/hello.h"

int main(int argc, char* argv[])
{
    Logger::getInstance().setInfoPath("D:\\test.log");
    Middleware* hello = new middleware::Hello(nullptr);
    HttpServer server(hello, "0.0.0.0", 8080);
    server.start();
    return 0;
}
