#include "Entity.h"

void Entity::setPositionAndRotation(glm::vec3 &position, glm::vec3 &rotation) {
    prevPosition = Entity::position;
    prevRotation = Entity::rotation;

    Entity::position = position;
    Entity::rotation = rotation;
}

void Entity::update() {

}

Entity::Entity(const char *luaFile) {
    luaState.open_libraries(sol::lib::base, sol::lib::io);

    luaState.script_file(luaFile);
}
