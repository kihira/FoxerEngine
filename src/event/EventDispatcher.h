
#ifndef GAMEENGINE301CR_EVENTDISPATCHER_H
#define GAMEENGINE301CR_EVENTDISPATCHER_H


#include <vector>
#include "EventListener.h"

template <typename T>
class EventDispatcher {
private:
    std::vector<EventListener<T>> listeners;
public:
    void push(T data);
};

template<typename T>
void EventDispatcher<T>::push(T data) {
    for (auto &listener : listeners) {
        if (listener.eventDispatched(data)) {
            return;
        }
    }
}


#endif //GAMEENGINE301CR_EVENTDISPATCHER_H
