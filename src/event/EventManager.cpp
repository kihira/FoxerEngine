
#include "EventManager.h"

EventManager::EventManager() = default;

EventManager::~EventManager() = default;

void EventManager::startUp() {
    logger = spdlog::stdout_color_mt("event");
}

void EventManager::shutDown() {

}
