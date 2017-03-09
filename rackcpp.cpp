#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <uv.h>
#include "middlewares/route.h"
#include "http.h"
#include "middleware.h"

void hello() {
	middleware::Route route;
	HTTPServer server(route, "127.0.0.1", 8080);
}