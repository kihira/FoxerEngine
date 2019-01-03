#ifndef GAMEENGINE301CR_EVENTNETWORKHANDLER_H
#define GAMEENGINE301CR_EVENTNETWORKHANDLER_H


#include "EventHandler.h"

/**
 * Class to handle sending and receiving events over the network
 * This class is only loaded once a server or client has been started
 */
class EventNetworkHandler : public EventHandler {
public:
    EventNetworkHandler();

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_EVENTNETWORKHANDLER_H