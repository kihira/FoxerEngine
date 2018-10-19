#include "Entity.h"

void Entity::setPositionAndRotation(glm::vec3 &position, glm::vec3 &rotation) {
    prevPosition = Entity::position;
    prevRotation = Entity::rotation;

    Entity::position = position;
    Entity::rotation = rotation;
}

void Entity::update() {
    sol::table entityTable = luaState["entity"];

    sol::function updateFn = entityTable["update"];
    updateFn();
}

Entity::Entity(const char *name) : name(name) {
}

void Entity::loadScript(const char * luaFile) {
    luaState.open_libraries(sol::lib::base, sol::lib::io);
    luaState.script_file(luaFile);

    sol::table entityTable = luaState["entity"];
    name = entityTable.get_or("name", name);
}
