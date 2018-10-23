#ifndef GAMEENGINE301CR_ENTITY_H
#define GAMEENGINE301CR_ENTITY_H

#include <glm/glm.hpp>
#include <sol.hpp>
#include "render/Mesh.h"
#include "render/Shader.h"


class Entity {
private:
    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 rotation;
    glm::vec3 prevRotation;
    glm::vec3 scale;
    glm::mat4 transform;
    std::string name;
    std::shared_ptr<Mesh> mesh;
    /*
     * Lua bindings
     */
    sol::protected_function updateFn;
public:
    Entity(std::string name);

    void update();

    void render(Shader *shader);

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getRotation() const;

    void setRotation(const glm::vec3 &rotation);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setMesh(const std::shared_ptr<Mesh> &mesh);
};


#endif //GAMEENGINE301CR_ENTITY_H
