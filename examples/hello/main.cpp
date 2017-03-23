#include <rackcpp.h>
#include <middlewares/hello.h>

int main(int argc, char *argv[]) {
    Middleware *hello = new middleware::Hello(nullptr);
    HttpServer server(hello, "0.0.0.0", 8080, true);
    server.start();
    return 0;
}
