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
    Mesh *mesh;
    sol::state luaState;
public:
    Entity(const char* luaFile);
    void setPositionAndRotation(glm::vec3 &position, glm::vec3 &rotation);
    void update();
    void render(Shader *shader);
};


#endif //GAMEENGINE301CR_ENTITY_H
