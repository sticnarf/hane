#include <string>
#include "middleware.h"
#include "http.h"
using namespace middleware;

HTTPServer::HTTPServer(Middleware &middleware, const std::string &bind_addr, int port) {
	middleware.call();
}