#pragma once
#include "../middleware.h"

namespace middleware {
	class Route : public Middleware {
	public:
		Route(Middleware *next_middleware);
		void call() override;
	};
}