
#ifndef GAMEENGINE301CR_EVENTLISTENER_H
#define GAMEENGINE301CR_EVENTLISTENER_H

#include "Event.h"

class EventHandler {
public:
    EventHandler();

    virtual ~EventHandler();

    virtual bool onEvent(Event &event);
};

#endif //GAMEENGINE301CR_EVENTLISTENER_H
