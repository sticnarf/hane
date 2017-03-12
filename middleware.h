#pragma once

namespace middleware {
	class Middleware {
	public:
		Middleware(Middleware *next_middleware) : next_middleware(next_middleware) { }
		virtual void call() = 0;

	protected:
		Middleware *next_middleware;
	};
}