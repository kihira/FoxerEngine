
#ifndef GAMEENGINE301CR_EVENT_H
#define GAMEENGINE301CR_EVENT_H

#include <map>
#include <string>
#include "../StringId.h"

enum EventType {
    EVENT_TYPE_LEVEL_STARTED = SID("EVENT_TYPE_LEVEL_STARTED"),
    EVENT_TYPE_PLAYER_SPAWNED = SID("EVENT_TYPE_PLAYER_SPAWNED")
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
    explicit Event(EventType eventType);

    EventType getType();

    template <typename T>
    void setArg(const std::string &name, T value);

    template <typename T>
    T getArg(const std::string &name);
};

#endif //GAMEENGINE301CR_EVENT_H
