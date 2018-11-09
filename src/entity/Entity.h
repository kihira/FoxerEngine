#ifndef GAMEENGINE301CR_ENTITY_H
#define GAMEENGINE301CR_ENTITY_H

#include <glm/glm.hpp>
#include <sol.hpp>
#include "../render/Mesh.h"
#include "../render/Shader.h"
#include "Component.h"


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
    std::vector<Component *> components;
public:
    Entity(unsigned short id, std::string name);

    void update();

    /**
     * Creates a copy of the entity. This should not be called directly unless you are EntityManager
     *
     * Used as part of the prototype pattern
     */
    std::shared_ptr<Entity> clone(unsigned short id);

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getRotation() const;

    const std::string &getName() const;

    void setRotation(const glm::vec3 &rotation);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setMesh(const std::shared_ptr<Mesh> &mesh);

    void setName(const std::string &name);

    void addComponent(Component *component);
};


#endif //GAMEENGINE301CR_ENTITY_H
