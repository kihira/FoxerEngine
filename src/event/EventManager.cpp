
#include "EventManager.h"

EventManager::EventManager() = default;

EventManager::~EventManager() = default;

void EventManager::startUp() {
    logger = spdlog::stdout_color_mt("event");
}

void EventManager::shutDown() {

}

void EventManager::registerHandler(EventType type, EventHandler handler) {
    if (handlers.find(type) == handlers.end()) {
        handlers.emplace(type, std::vector<EventHandler>());
    }
    handlers[type].emplace_back(handler);
}

void EventManager::push(Event &event) {
    for (auto &handler : handlers[event.getType()]) {
        if (handler.onEvent(event)) {
            break;
        }
    }
}
