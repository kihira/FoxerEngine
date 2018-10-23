
#ifndef GAMEENGINE301CR_LEVEL_H
#define GAMEENGINE301CR_LEVEL_H

#include <string>
#include <sol.hpp>


class Level {
private:
    std::string name;
    sol::protected_function updateFn;
public:
    void update();
};


#endif //GAMEENGINE301CR_LEVEL_H
