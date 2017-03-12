#include "../middleware.h"
#include "route.h"
#include <iostream>

namespace middleware {
	Route::Route(Middleware *next_middleware) : Middleware(next_middleware) { }

	void Route::call() {
		std::cout << "Hello!" << std::endl;
	}
}