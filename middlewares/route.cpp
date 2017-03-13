#include "../middleware.h"
#include "route.h"
#include <iostream>

namespace middleware {
    Route::Route(Middleware *next_middleware) : next_middleware(next_middleware) {}

	void Route::call() {
		std::cout << "Hello!" << std::endl;
	}

	Route::~Route() {
        if(next_middleware)
            delete next_middleware;
	}
}