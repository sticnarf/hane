#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <uv.h>
#include "middlewares/route.h"
#include "http.h"
#include "middleware.h"

void hello() {
    HttpServer server(new middleware::Route(nullptr), "127.0.0.1", 8080);
    server.start();
}