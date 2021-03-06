
#ifndef FOXERENGINE_EVENTMANAGER_H
#define FOXERENGINE_EVENTMANAGER_H

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
    std::vector<EventHandler *> globalHandlers;
    std::map<StringId, std::vector<EventHandler *>> handlers;
    std::map<StringId, EventMeta> eventsMeta; // Stores information about what should be in an event. Used for networking
    EventNetworkHandler *networkHandler;
public:
    EventManager();

    virtual ~EventManager();

    void startUp();

    void shutDown();

    /**
     * Registers an event handler for ALL events.
     * Global handlers have priority over standard handlers and can prevent an event from filtering down
     * This should be used sparingly as many events are often fired per update.
     * @param handler Pointer to the handler
     */
    void registerHandler(EventHandler *handler);

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
     * Deregisters an event handler for a specific event
     * @param type The event
     * @param handler The handler
     */
    void deregisterHandler(StringId type, EventHandler *handler);

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


#endif //FOXERENGINE_EVENTMANAGER_H
