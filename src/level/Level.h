
#ifndef FOXERENGINE_LEVEL_H
#define FOXERENGINE_LEVEL_H

#include <string>
#include <sol.hpp>
#include "../util/StringId.h"
#include "../event/EventHandler.h"


class Level : public EventHandler {
private:
    const StringId id;
    std::string name;
    sol::function updateFn;
    sol::function onEventFn;

    std::vector<StringId> events;
    std::unordered_map<std::string, sol::object> entries;
public:
    explicit Level(StringId id);

    virtual ~Level();

    void update();

    const std::string &getName() const;

    void setName(const std::string &name);

    void setUpdateFn(const sol::function &updateFn);

    void setOnEventFn(const sol::function &onEventFn);

    void setEvents(const std::vector<StringId> &events);

    const std::vector<StringId> &getEvents() const;

    bool canHandleEvents();

    StringId getId() const;

    /**
     * Used by Sol/Lua to dynamically set values on the level.
     * Allows for a large amount of freedom within scripting
     * @param key The key name
     * @param value The key value
     */
    void dynamicSet(std::string key, sol::stack_object value);

    /**
     * Used by Sol/Lua to dynamically get values on the level.
     * Allows for a large amount of freedom within scripti
     * @param key The key name
     * @return The value
     */
    sol::object dynamicGet(std::string key);

    bool onEvent(Event &event) override;
};


#endif //FOXERENGINE_LEVEL_H
