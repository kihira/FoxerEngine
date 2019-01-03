
#ifndef GAMEENGINE301CR_EVENT_H
#define GAMEENGINE301CR_EVENT_H

#include <map>
#include <string>
#include "../util/StringId.h"


struct Variant {
    enum Type {
        TYPE_INTEGER,
        TYPE_UNSIGNED_SHORT,
        TYPE_SHORT,
        TYPE_FLOAT,
        TYPE_BOOL,
        TYPE_STRING_ID,
        TYPE_COUNT
    };

    Type type;

    union {
        int asInt;
        unsigned short asUShort;
        short asShort;
        float asFloat;
        bool asBool;
        StringId asStringId;
    };
};

/**
 * A general class to hold data about an event
 */
class Event {
private:
    StringId type;
    std::map<std::string, Variant> arguments;
public:
    explicit Event(StringId type);

    explicit Event(const char *type);

    StringId getType();

    /**
     * Pushes the event into the event queue and prevents further changes.
     * This is on the event itself so the event manager header file doesn't need to be included everywhere
     */
    void push();

    // todo convert all the const char into std::string
    template <typename T>
    void setArg(const char *name, T value);

    template <typename T>
    T getArg(const char *name);

    const std::map<std::string, Variant> &getArguments() const;

    unsigned int serialise(void *&data);

    void deserialise(void *data);
};

#endif //GAMEENGINE301CR_EVENT_H
