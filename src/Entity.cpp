#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include "vectors.h"

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

void Entity::loadScript(sol::state &luaState, const char *file) {
    luaState.open_libraries(sol::lib::base, sol::lib::io);
    auto loadResult = luaState.load_file(file);
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
    name = entityTable["name"].get_or(name);
    updateFn = entityTable["update"];

    std::string mesh = entityTable["mesh"].get_or(std::string("ERROR")); // todo
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
