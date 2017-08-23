#include "session_middleware.hpp"
#include <string>
#include <http/session/session_base.hpp>

SessionMiddleware::SessionMiddleware(MiddlewarePtr nextMiddleware) : SimpleMiddleware(nextMiddleware) {
}

void SessionMiddleware::process(Request &req, std::shared_ptr<Response> &resp) {
    const std::string SESSION_COOKIE_NAME = "HANE_SESSIONID";

    auto &sessionBase = SessionBase::getInstance();

    auto cookies = req.getCookies();
    auto sessionId = cookies->find(SESSION_COOKIE_NAME);

    SessionPtr session;

    if (sessionId == cookies->end()) {
        session = sessionBase.newSession();
        resp->cookies.putCookie({SESSION_COOKIE_NAME, session->getId()});
    } else {
        session = sessionBase.getSession(sessionId->second.value);
        if (!session) {
            session = sessionBase.newSession();
            resp->cookies.putCookie({SESSION_COOKIE_NAME, session->getId()});
        }
    }

    req.session = session;
}
