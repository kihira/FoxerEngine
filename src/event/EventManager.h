
#ifndef GAMEENGINE301CR_EVENTMANAGER_H
#define GAMEENGINE301CR_EVENTMANAGER_H

#include <spdlog/spdlog.h>
#include <map>
#include "EventHandler.h"


class EventManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<StringId, std::vector<EventHandler *>> handlers;
public:
    EventManager();

    virtual ~EventManager();

    void startUp();

    void shutDown();

    void registerHandler(StringId type, EventHandler *handler);

    void registerHandler(const std::vector<StringId> &events, EventHandler *handler);

    void push(Event &event);
};


#endif //GAMEENGINE301CR_EVENTMANAGER_H
