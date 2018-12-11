
#ifndef GAMEENGINE301CR_EVENTLISTENER_H
#define GAMEENGINE301CR_EVENTLISTENER_H

template <typename T>
class EventListener {
    virtual bool eventReceived(T data) = 0;
};

#endif //GAMEENGINE301CR_EVENTLISTENER_H
