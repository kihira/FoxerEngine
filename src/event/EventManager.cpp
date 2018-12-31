
#include "EventManager.h"

EventManager::EventManager() = default;

EventManager::~EventManager() = default;

void EventManager::startUp() {
    logger = spdlog::get("main")->clone("event");

    // Register some default events
    handlers.emplace(SID("EVENT_TYPE_LEVEL_STARTED"), std::vector<EventHandler *>());
    handlers.emplace(SID("EVENT_TYPE_PLAYER_SPAWNED"), std::vector<EventHandler *>());
}

void EventManager::shutDown() {

}

void EventManager::registerHandler(StringId type, EventHandler *handler) {
    if (handlers.find(type) == handlers.end()) {
        handlers.emplace(type, std::vector<EventHandler *>());
    }
    handlers[type].push_back(handler);
    logger->debug("Registered event handler for {:d}", type);
}

void EventManager::registerHandler(const std::vector<StringId> &events, EventHandler *handler) {
    for (auto event : events) {
        registerHandler(event, handler);
    }
}


void EventManager::push(Event &event) {
    for (auto handler : handlers[event.getType()]) {
        if (handler->onEvent(event)) {
            break;
        }
    }
}
