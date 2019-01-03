
#ifndef GAMEENGINE301CR_LEVEL_H
#define GAMEENGINE301CR_LEVEL_H

#include <string>
#include <sol.hpp>
#include "../util/StringId.h"
#include "../event/EventHandler.h"


class Level : public EventHandler {
private:
    const StringId id;
    std::string name;
    sol::protected_function updateFn;
    sol::function onEventFn;

    std::vector<StringId> events;
public:
    explicit Level(StringId id);

    virtual ~Level();

    void update();

    const std::string &getName() const;

    void setName(const std::string &name);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setOnEventFn(const sol::function &onEventFn);

    void setEvents(const std::vector<StringId> &events);

    const std::vector<StringId> &getEvents() const;

    bool canHandleEvents();

    StringId getId() const;

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_LEVEL_H
