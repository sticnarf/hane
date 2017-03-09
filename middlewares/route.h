#pragma once
#include "../middleware.h"

namespace middleware {
	class Route : public Middleware {
	public:
		void call() override;
	};
}