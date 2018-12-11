
#ifndef GAMEENGINE301CR_EVENTDISPATCHER_H
#define GAMEENGINE301CR_EVENTDISPATCHER_H


#include <vector>
#include "EventListener.h"

template <class T>
class EventDispatcher {
private:
    std::vector<EventListener<T>> listeners;
public:
    void push(T data);
};

template<class T>
void EventDispatcher<T>::push(T data) {
    for (auto &listener : listeners) {
        if (listener.eventDispatched(data)) {
            return;
        }
    }
}


#endif //GAMEENGINE301CR_EVENTDISPATCHER_H
