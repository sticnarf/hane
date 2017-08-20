#include <random>
#include "../../utils/logger.hpp"
#include "session_base.hpp"

SessionBase &SessionBase::getInstance() {
    static SessionBase base;
    return base;
}

SessionPtr SessionBase::newSession() {
    std::uniform_int_distribution<unsigned long long> dis;

    auto d1 = dis(gen), d2 = dis(gen);
    std::string id = fmt::format("{0:0=16x}{1:0=16x}", d1, d2);

    auto session = std::make_shared<Session>(id);

    (*newGen)[id] = session;

    return session;
}

SessionBase::SessionBase() : gen(std::random_device()()) {
    oldGen = std::make_shared<std::unordered_map<std::string, SessionPtr>>();
    newGen = std::make_shared<std::unordered_map<std::string, SessionPtr>>();
    updateExpires();
}

void SessionBase::updateExpires() {
    expires = std::chrono::system_clock::to_time_t(
            std::chrono::system_clock::now() +
            std::chrono::hours(2));
}

void SessionBase::checkUpdateGen() {
    auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    if (now > expires)
        updateGen();
}

void SessionBase::updateGen() {
    oldGen = newGen;
    newGen = std::make_shared<std::unordered_map<std::string, SessionPtr>>();
    updateExpires();
}

SessionPtr SessionBase::getSession(const std::string &sessionId) {
    checkUpdateGen();

    auto entry = newGen->find(sessionId);
    if (entry != newGen->end()) {
        return entry->second;
    }

    auto oldEntry = oldGen->find(sessionId);
    if (oldEntry == oldGen->end()) {
        return nullptr;
    }

    (*newGen)[sessionId] = oldEntry->second;

    return oldEntry->second;
}

void SessionBase::deleteSession(const std::string &sessionId) {
    checkUpdateGen();

    auto entry = newGen->find(sessionId);
    if (entry != newGen->end()) {
        newGen->erase(entry);
    }

    auto oldEntry = oldGen->find(sessionId);
    if (oldEntry != oldGen->end()) {
        oldGen->erase(oldEntry);
    }
}

const std::string &Session::getId() const {
    return id;
}

Session::Session(const std::string &id) : id(id) {}
