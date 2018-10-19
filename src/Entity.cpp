#include "Entity.h"

void Entity::setPositionAndRotation(glm::vec3 &position, glm::vec3 &rotation) {
    prevPosition = Entity::position;
    prevRotation = Entity::rotation;

    Entity::position = position;
    Entity::rotation = rotation;
}

void Entity::update() {
    if (updateFn != sol::lua_nil) {
        auto updateResult = updateFn();
        if (!updateResult.valid()) {
            sol::error err = updateResult;
            std::cerr << "Error updating Entity (" << name << "):" << std::endl
                      << err.what() << std::endl;
        }
    }
}

Entity::Entity(const char *name) : name(name) {
}

void Entity::loadScript(const char * luaFile) {
    luaState.open_libraries(sol::lib::base, sol::lib::io);
    auto loadResult = luaState.load_file(luaFile);
    if (loadResult.status() != sol::load_status::ok) {
        std::cerr << "Failed to load lua file for entity " << name << std::endl;
        return;
    }
    sol::protected_function_result loadRunResult = loadResult();
    if (!loadRunResult.valid()) {
        sol::error err = loadRunResult;
        std::cerr << "Failed to run lua file for entity " << name << ": " << std::endl
                  << err.what() << std::endl;
    }

    // Load data from lua file and bind functions
    sol::table entityTable = luaState["entity"];
    name = entityTable.get_or("name", name);
    updateFn = entityTable["update"];
}
