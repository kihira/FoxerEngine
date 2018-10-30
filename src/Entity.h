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
    std::shared_ptr<Mesh> mesh;
    /*
     * Lua bindings
     */
    sol::protected_function updateFn;

    const unsigned short id;
    std::string name;
public:
    Entity(unsigned short id, std::string name);

    void update();

    void render(Shader *shader);

    /**
     * Creates a copy of the entity. This should not be called directly unless you are EntityManager
     *
     * Used as part of the prototype pattern
     */
    std::shared_ptr<Entity> clone(unsigned short id);

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getRotation() const;

    void setRotation(const glm::vec3 &rotation);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setMesh(const std::shared_ptr<Mesh> &mesh);
};


#endif //GAMEENGINE301CR_ENTITY_H
