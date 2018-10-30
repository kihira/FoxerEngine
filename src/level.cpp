
#include "level.h"

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
