
#ifndef GAMEENGINE301CR_EVENTMANAGER_H
#define GAMEENGINE301CR_EVENTMANAGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <map>
#include "EventHandler.h"


class EventManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<EventType, std::vector<EventHandler>> handlers;
public:
    EventManager();

    virtual ~EventManager();

    void startUp();

    void shutDown();

    void registerHandler(EventType type, EventHandler handler);

    void push(Event &event);
};


#endif //GAMEENGINE301CR_EVENTMANAGER_H
