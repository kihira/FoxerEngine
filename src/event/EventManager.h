
#ifndef GAMEENGINE301CR_EVENTMANAGER_H
#define GAMEENGINE301CR_EVENTMANAGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <typeindex>
#include <map>
#include "EventListener.h"
#include "EventDispatcher.h"


class EventManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<std::type_index, EventDispatcher *> dispatchers;
public:
    EventManager();

    virtual ~EventManager();

    void startUp();

    void shutDown();

    template <typename T>
    void registerEventType(unsigned short eventId, T);

    template <typename T>
    void registerEventListener(EventListener<T> listener);

    template <typename T>
    void push(T data);
};


#endif //GAMEENGINE301CR_EVENTMANAGER_H
