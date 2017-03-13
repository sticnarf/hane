#include "../middleware.h"
#include "route.h"
#include <iostream>

namespace middleware {
    Route::Route(Middleware *next_middleware) {
        this->next_middleware = next_middleware;
    }

    void Route::call(const Request &req, Response &resp) {
        std::cout << "Hello!" << std::endl;
    }

    Route::~Route() {
        if (next_middleware)
            delete next_middleware;
    }
}