#pragma once

namespace middleware {
	class Middleware {
	public:
		virtual void call() = 0;
	};
}