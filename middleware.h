#pragma once

namespace middleware {
	class Middleware {
	public:
		virtual void call() = 0;
		virtual ~Middleware();

	protected:
		Middleware *next_middleware;
	};
}