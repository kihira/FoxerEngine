#ifndef GAMEENGINE301CR_ENTITY_H
#define GAMEENGINE301CR_ENTITY_H

#include <glm/glm.hpp>
#include <sol.hpp>
#include "render/Mesh.h"
#include "render/Shader.h"


class Entity {
private:
    std::string name;
    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 rotation;
    glm::vec3 prevRotation;
    Mesh *mesh;
    /*
     * Lua bindings
     */
    sol::state luaState;
    sol::protected_function updateFn;
public:
    Entity(const char *name);
    void setPositionAndRotation(glm::vec3 &position, glm::vec3 &rotation);
    void loadScript(sol::state &luaState, const char *file);
    void update();
    void render(Shader *shader);
};


#endif //GAMEENGINE301CR_ENTITY_H
