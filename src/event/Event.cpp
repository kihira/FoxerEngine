#include <spdlog/spdlog.h>
#include "Event.h"
#include "../Managers.h"
#include "EventManager.h"

Event::Event(StringId type) : type(type) {}

Event::Event(const char *type) {
    Event::type = processString(type);
}

StringId Event::getType() {
    return type;
}

template<typename T>
void Event::setArg(const char *name, T value) {
    spdlog::get("event")->error("Attempting to set an unsupported arg {}: {}", name, value);
}

template<typename T>
T Event::getArg(const char *name) {
    spdlog::get("event")->error("Attempting to get an unsupported arg {}", name);
    return nullptr;
}

void Event::push() {
    gEventManager.push(*this);
}

unsigned int Event::serialise(void *&data) {
    auto size = sizeof(StringId) * arguments.size() + sizeof(StringId);
    auto eventData = static_cast<StringId *>(malloc(size)); // todo can just serialise the exact length of the data but this works for now
    eventData[0] = type;

    // We only serialise argument values, we know argument names from the asset metadata
    auto eventMeta = gEventManager.getEventMeta(type);
    unsigned short pos = 1;

    for (auto &argName : eventMeta.args) {
        auto argValue = arguments[argName];
        eventData[pos] = argValue.asStringId;
        pos += 1;
    }

    data = eventData;
    return size;
}

void Event::deserialise(void *data) {
    auto argData = static_cast<StringId *>(data);
    type = *argData;
    auto eventMeta = gEventManager.getEventMeta(*static_cast<StringId *>(data));
    argData += 1;

    for (auto &argName : eventMeta.args) {
        // todo use proper type
        Variant variant{};
        variant.type = Variant::TYPE_STRING_ID;
        variant.asStringId = *argData;
        arguments.emplace(argName, variant);
        argData += 1;
    }
}

const std::map<std::string, Variant> &Event::getArguments() const {
    return arguments;
}

template<>
void Event::setArg<float>(const char *name, float value) {
    Variant variant{};
    variant.type = Variant::TYPE_FLOAT;
    variant.asFloat = value;

    arguments.emplace(name, variant);
}

template<>
void Event::setArg<int>(const char *name, int value) {
    Variant variant{};
    variant.type = Variant::TYPE_INTEGER;
    variant.asInt = value;

    arguments.emplace(name, variant);
}

template<>
void Event::setArg<unsigned short>(const char *name, unsigned short value) {
    Variant variant{};
    variant.type = Variant::TYPE_UNSIGNED_SHORT;
    variant.asUShort = value;

    arguments.emplace(name, variant);
}

template<>
void Event::setArg<short>(const char *name, short value) {
    Variant variant{};
    variant.type = Variant::TYPE_SHORT;
    variant.asShort = value;

    arguments.emplace(name, variant);
}

template<>
void Event::setArg<bool>(const char *name, bool value) {
    Variant variant{};
    variant.type = Variant::TYPE_BOOL;
    variant.asBool = value;

    arguments.emplace(name, variant);
}

template<>
void Event::setArg<StringId>(const char *name, StringId value) {
    Variant variant{};
    variant.type = Variant::TYPE_STRING_ID;
    variant.asStringId = value;

    arguments.emplace(name, variant);
}

// todo error checking
template<>
float Event::getArg<float>(const char *name) {
    return arguments[name].asFloat;
}

template<>
int Event::getArg<int>(const char *name) {
    return arguments[name].asInt;
}

template<>
unsigned short Event::getArg<unsigned short>(const char *name) {
    return arguments[name].asUShort;
}

template<>
short Event::getArg<short>(const char *name) {
    return arguments[name].asShort;
}

template<>
bool Event::getArg<bool>(const char *name) {
    return arguments[name].asBool;
}

template<>
StringId Event::getArg<StringId>(const char *name) {
    return arguments[name].asStringId;
}