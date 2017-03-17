#include "middleware.h"

namespace middleware {
    void Middleware::call(const Request &req, Response &resp) {
        process(req, resp);
        if (next_middleware)
            next_middleware->call(req, resp);
    }
}
