
#ifndef GAMEENGINE301CR_EVENTMANAGER_H
#define GAMEENGINE301CR_EVENTMANAGER_H

#include <spdlog/spdlog.h>
#include <map>
#include "EventHandler.h"
#include "EventNetworkHandler.h"

struct EventMeta {
    std::vector<std::string> args;
};

class EventManager : public EventHandler {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<StringId, std::vector<EventHandler *>> handlers;
    std::map<StringId, EventMeta> eventsMeta; // Stores information about what should be in an event. Used for networking
    EventNetworkHandler *networkHandler;
public:
    EventManager();

    virtual ~EventManager();

    void startUp();

    void shutDown();

    /**
     * Registers an event handler that will be called when that is fired.
     * @param type The event to listen for
     * @param handler Pointer to the handler
     */
    void registerHandler(StringId type, EventHandler *handler);

    /**
     * Registers an event handler that will be called when any events in the vector are fired.
     * @param events The event(s) to listen for
     * @param handler Pointer to the handler
     */
    void registerHandler(const std::vector<StringId> &events, EventHandler *handler);

    /**
     * Pushes an event into the queue to be resolved immediately
     * @param event The event
     */
    void push(Event &event);

    const EventMeta getEventMeta(StringId id);

    /**
     * Stores information about events that is then used serialisation
     * @param eventMetas
     */
    void loadEvents(std::map<StringId, EventMeta> eventMetas);

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_EVENTMANAGER_H
