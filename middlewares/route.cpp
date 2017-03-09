#include "../middleware.h"
#include "route.h"
#include <iostream>

namespace middleware {
	void Route::call() {
		std::cout << "Hello!" << std::endl;
	}
}