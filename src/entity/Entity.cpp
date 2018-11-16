#include <utility>

#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include "../vectors.h"

Entity::Entity(const unsigned short id, std::string name) : id(id), name(std::move(name)) {}

void Entity::update() {
    if (updateFn != sol::lua_nil) {
        auto updateResult = updateFn(this);
        if (!updateResult.valid()) {
            sol::error err = updateResult;
            std::cerr << "Error updating Entity (" << name << "):" << std::endl
                      << err.what() << std::endl;
        }
    }
}

const glm::vec3 &Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const glm::vec3 &position) {
    Entity::position = position;
}

const glm::vec3 &Entity::getRotation() const {
    return rotation;
}

const std::string &Entity::getName() const {
    return name;
}

void Entity::setRotation(const glm::vec3 &rotation) {
    Entity::rotation = rotation;
}

void Entity::setUpdateFn(const sol::protected_function &updateFn) {
    Entity::updateFn = updateFn;
}

// todo need to clone components
std::shared_ptr<Entity> Entity::clone(const unsigned short id) {
    auto newEntity = std::make_shared<Entity>(id, name);

    newEntity->setPosition(position);
    newEntity->setRotation(rotation);
    newEntity->setUpdateFn(updateFn);
    return newEntity;
}

void Entity::setName(const std::string &name) {
    Entity::name = name;
}

void Entity::addComponent(Component *component) {
    components.push_back(component);
}

const glm::vec3 &Entity::getPrevPosition() const {
    return prevPosition;
}

const glm::vec3 &Entity::getPrevRotation() const {
    return prevRotation;
}

const unsigned short Entity::getId() const {
    return id;
}
