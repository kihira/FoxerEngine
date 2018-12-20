
#ifndef GAMEENGINE301CR_EVENTLISTENER_H
#define GAMEENGINE301CR_EVENTLISTENER_H

#include "Event.h"

class EventHandler {
public:
    virtual bool onEvent(Event &event) { return false; };
};

#endif //GAMEENGINE301CR_EVENTLISTENER_H
