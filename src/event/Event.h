
#ifndef GAMEENGINE301CR_EVENT_H
#define GAMEENGINE301CR_EVENT_H

#include <map>
#include <string>
#include "../util/StringId.h"


struct Variant {
    enum Type {
        TYPE_INTEGER,
        TYPE_SHORT,
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
    StringId type;
    std::map<std::string, Variant> arguments;
public:
    explicit Event(StringId type);

    StringId getType();

    /**
     * Pushes the event into the event queue and prevents further changes.
     * This is on the event itself so the event manager header file doesn't need to be included everywhere
     */
    void push();

    template <typename T>
    void setArg(const std::string &name, T value);

    template <typename T>
    T getArg(const std::string &name);
};

#endif //GAMEENGINE301CR_EVENT_H
