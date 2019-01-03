
#include "Level.h"

Level::Level(StringId id) : id(id) {}

Level::~Level() {}

void Level::update() {
    if (updateFn != sol::lua_nil) {
        auto updateResult = updateFn(this);
        if (!updateResult.valid()) {
            sol::error err = updateResult;
            std::cerr << "Error updating Level (" << name << "):" << std::endl
                      << err.what() << std::endl;
        }
    }
}

void Level::setUpdateFn(const sol::protected_function &updateFn) {
    Level::updateFn = updateFn;
}

const std::string &Level::getName() const {
    return name;
}

void Level::setName(const std::string &name) {
    Level::name = name;
}

void Level::setOnEventFn(const sol::function &onEventFn) {
    Level::onEventFn = onEventFn;
}

void Level::setEvents(const std::vector<StringId> &events) {
    Level::events = events;
}

const std::vector<StringId> &Level::getEvents() const {
    return events;
}

bool Level::canHandleEvents() {
    return onEventFn != sol::lua_nil;
}

StringId Level::getId() const {
    return id;
}


void Level::dynamicSet(std::string key, sol::stack_object value) {
    auto it = entries.find(key);
    if (it == entries.cend()) {
        entries.insert(it, { std::move(key), std::move(value) });
    }
    else {
        std::pair<const std::string, sol::object>& kvp = *it;
        sol::object& entry = kvp.second;
        entry = sol::object(std::move(value));
    }
}

sol::object Level::dynamicGet(std::string key) {
    auto it = entries.find(key);
    if (it == entries.cend()) {
        return sol::lua_nil;
    }
    return it->second;
}

bool Level::onEvent(Event &event) {
    // We can make the assumption that this would never be registered as an event handler if onEventFn is null
    return onEventFn(this, event);
}