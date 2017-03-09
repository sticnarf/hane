#pragma once
#include <string>
#include "middleware.h"
using namespace middleware;

class HTTPServer {
public:
	HTTPServer(Middleware &middleware, const std::string &bind_addr, int port);
};