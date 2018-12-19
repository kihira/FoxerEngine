
#ifndef GAMEENGINE301CR_EVENTMANAGER_H
#define GAMEENGINE301CR_EVENTMANAGER_H

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <typeindex>
#include <map>
#include "EventListener.h"
#include "EventDispatcher.h"

typedef void (* EventCallback)(void *data);


class EventManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<std::type_index, std::vector<EventCallback>> dispatchers;
public:
    EventManager();

    virtual ~EventManager();

    void startUp();

    void shutDown();

    template <typename T>
    void registerEventType() {
        logger->debug("Registered event type {}", std::type_index(typeid(T)));
        dispatchers.emplace(std::type_index(typeid(T)), std::vector<EventCallback>());
    }

    template <typename T>
    void registerListener(EventCallback callback) {
        dispatchers[std::type_index(typeid(T))].emplace_back(callback);
    }

    template <typename T>
    void push(T data) {
        for (auto &listener : dispatchers[std::type_index(typeid(T))]) {
            listener(data);
        }
    }
};


#endif //GAMEENGINE301CR_EVENTMANAGER_H
