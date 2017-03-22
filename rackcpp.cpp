#include "middlewares/route.h"
#include "http.h"

void hello() {
    Middleware *middleware = new middleware::Route(nullptr);
    HttpServer *server = new HttpServer(middleware, "0.0.0.0", 8080, true);
    server->start();
}
