
#ifndef FOXERENGINE_EVENTLISTENER_H
#define FOXERENGINE_EVENTLISTENER_H

#include "Event.h"

class EventHandler {
public:
    virtual bool onEvent(Event &event) = 0;
};

#endif //FOXERENGINE_EVENTLISTENER_H
