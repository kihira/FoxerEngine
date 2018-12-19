
#ifndef GAMEENGINE301CR_EVENTLISTENER_H
#define GAMEENGINE301CR_EVENTLISTENER_H

#include "Event.h"

class EventHandler {
public:
    virtual bool onEvent(Event &event) = 0;
};

#endif //GAMEENGINE301CR_EVENTLISTENER_H
