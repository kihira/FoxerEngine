
#ifndef GAMEENGINE301CR_EVENT_H
#define GAMEENGINE301CR_EVENT_H

enum EventType {
    EVENT_TYPE_LEVEL_STARTED
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
        unsigned int asStringId;
    };
};

class Event {
    EventType eventType;
    std::map<std::string, Variant> arguments;
};

#endif //GAMEENGINE301CR_EVENT_H
