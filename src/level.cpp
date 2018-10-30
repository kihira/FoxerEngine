
#include "level.h"

void Level::update() {
    if (updateFn != sol::lua_nil) {
        auto updateResult = updateFn();
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
