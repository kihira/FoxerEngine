#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include "vectors.h"

Entity::Entity(const char *name) : name(name) {}

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

void Entity::render(Shader *shader) {
    transform = glm::translate(glm::mat4(1.f), position);
    transform = glm::rotate(transform, rotation.x, vector::RIGHT);
    transform = glm::rotate(transform, rotation.y, vector::UP);
    transform = glm::rotate(transform, rotation.z, vector::BACKWARD);
    transform = glm::scale(transform, scale);

    shader->setUniform("model", transform);
    mesh->render();
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

void Entity::setRotation(const glm::vec3 &rotation) {
    Entity::rotation = rotation;
}

void Entity::setUpdateFn(const sol::protected_function &updateFn) {
    Entity::updateFn = updateFn;
}

void Entity::setMesh(const std::shared_ptr<Mesh> &mesh) {
    Entity::mesh = mesh;
}
