#ifndef HANE_SESSION_BASE_HPP
#define HANE_SESSION_BASE_HPP

#include <unordered_map>
#include <map>
#include <memory>
#include <string>
#include <chrono>

class Session : public std::map<std::string, std::shared_ptr<void>> {
    std::string id;

    friend class SessionBase;

public:
    explicit Session(const std::string &id);

    const std::string &getId() const;
};

typedef std::shared_ptr<Session> SessionPtr;

class SessionBase {
    std::mt19937 gen;

    std::time_t expires;
    std::shared_ptr<std::unordered_map<std::string, SessionPtr>> oldGen;
    std::shared_ptr<std::unordered_map<std::string, SessionPtr>> newGen;

    void checkUpdateGen();

    void updateGen();

    void updateExpires();

public:
    static SessionBase &getInstance();

    SessionBase();

    SessionPtr newSession();

    /**
     * Get session by id
     * @param sessionId
     * @return the corresponding session pointer if found,
     *         otherwise nullptr is returned
     */
    SessionPtr getSession(const std::string &sessionId);

    void deleteSession(const std::string &sessionId);
};

#endif
