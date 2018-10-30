
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

    const std::string &getName() const;

    void setName(const std::string &name);

    void setUpdateFn(const sol::protected_function &updateFn);

};


#endif //GAMEENGINE301CR_LEVEL_H
