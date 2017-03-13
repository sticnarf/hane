#include "middlewares/route.h"
#include "http.h"

void hello() {
    HttpServer server(new middleware::Route(nullptr), "127.0.0.1", 8080);
    server.start();
}