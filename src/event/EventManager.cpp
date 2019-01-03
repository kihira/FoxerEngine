#include <utility>


#include "EventManager.h"

EventManager::EventManager() = default;

EventManager::~EventManager() = default;

void EventManager::startUp() {
    logger = spdlog::get("main")->clone("event");

    registerHandler(SID("EVENT_TYPE_SERVER_START"), this);
    registerHandler(SID("EVENT_TYPE_CLIENT_START"), this);
}

void EventManager::shutDown() {
    delete networkHandler;
}

void EventManager::registerHandler(EventHandler *handler) {
    globalHandlers.push_back(handler);
    logger->debug("Registered global event handler");
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
    SPDLOG_LOGGER_DEBUG(logger, "Pushing event {}", event.getType());
    for (auto handler : globalHandlers) {
        if (handler->onEvent(event)) {
            return;
        }
    }

    for (auto handler : handlers[event.getType()]) {
        if (handler->onEvent(event)) {
            return;
        }
    }
}

bool EventManager::onEvent(Event &event) {
    switch (event.getType()) {
        case SID("EVENT_TYPE_SERVER_START"): {
            networkHandler = new EventNetworkHandler();
        }
        case SID("EVENT_TYPE_CLIENT_START"): {
            networkHandler = new EventNetworkHandler();
        }
    }
    return false;
}

const EventMeta EventManager::getEventMeta(StringId id) {
    return eventsMeta[id];
}

void EventManager::loadEvents(std::map<StringId, EventMeta> eventMetas) {
    EventManager::eventsMeta = std::move(eventMetas);
}
