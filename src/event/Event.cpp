#include "Event.h"

Event::Event(StringId type) : type(type) {}

StringId Event::getType() {
    return type;
}

template<typename T>
void Event::setArg(const std::string &name, T value) {

}

template<typename T>
T Event::getArg(const std::string &name) {
    return nullptr;
}

template<>
void Event::setArg<float>(const std::string &name, float value) {
    Variant variant{};
    variant.type = Variant::TYPE_FLOAT;
    variant.asFloat = value;

    arguments[name] = variant;
}

template<>
void Event::setArg<int>(const std::string &name, int value) {
    Variant variant{};
    variant.type = Variant::TYPE_INTEGER;
    variant.asInteger = value;

    arguments[name] = variant;
}

template<>
void Event::setArg<bool>(const std::string &name, bool value) {
    Variant variant{};
    variant.type = Variant::TYPE_BOOL;
    variant.asBool = value;

    arguments[name] = variant;
}

template<>
void Event::setArg<StringId>(const std::string &name, StringId value) {
    Variant variant{};
    variant.type = Variant::TYPE_STRING_ID;
    variant.asStringId = value;

    arguments[name] = variant;
}

// todo error checking
template<>
float Event::getArg<float>(const std::string &name) {
    return arguments[name].asFloat;
}

template<>
int Event::getArg<int>(const std::string &name) {
    return arguments[name].asInteger;
}

template<>
bool Event::getArg<bool>(const std::string &name) {
    return arguments[name].asBool;
}

template<>
StringId Event::getArg<StringId>(const std::string &name) {
    return arguments[name].asStringId;
}