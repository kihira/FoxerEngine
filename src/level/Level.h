
#ifndef GAMEENGINE301CR_LEVEL_H
#define GAMEENGINE301CR_LEVEL_H

#include <string>
#include <sol.hpp>
#include "../util/StringId.h"


class Level {
private:
    std::string name;
    sol::protected_function updateFn;
    sol::function onEventFn;

    std::vector<StringId> events;
public:
    void update();

    const std::string &getName() const;

    void setName(const std::string &name);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setOnEventFn(const sol::function &onEventFn);

    void setEvents(const std::vector<StringId> &events);
};


#endif //GAMEENGINE301CR_LEVEL_H
