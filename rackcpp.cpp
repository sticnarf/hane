#include "middlewares/route.h"
#include "http.h"

void hello() {
    Middleware *middleware = new middleware::Route(nullptr);
    HttpServer *server = new HttpServer(middleware, "127.0.0.1", 8080);
    server->start();
}