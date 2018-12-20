
#ifndef GAMEENGINE301CR_EVENT_H
#define GAMEENGINE301CR_EVENT_H

#include "../StringId.h"

enum EventType {
    EVENT_TYPE_LEVEL_STARTED = SID("EVENT_TYPE_LEVEL_STARTED")
};

struct Variant {
    enum Type {
        TYPE_INTEGER,
        TYPE_FLOAT,
        TYPE_BOOL,
        TYPE_STRING_ID,
        TYPE_COUNT
    };

    Type type;

    union {
        int asInteger;
        float asFloat;
        bool asBool;
        StringId asStringId;
    };
};

class Event {
private:
    EventType eventType;
    std::map<std::string, Variant> arguments;

public:
    EventType getType() const {
        return eventType;
    }
};

#endif //GAMEENGINE301CR_EVENT_H
