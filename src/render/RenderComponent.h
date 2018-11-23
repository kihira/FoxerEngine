
#ifndef GAMEENGINE301CR_RENDERCOMPONENT_H
#define GAMEENGINE301CR_RENDERCOMPONENT_H


#include "../entity/Component.h"
#include "Mesh.h"
#include "Shader.h"

class RenderComponent : public Component {
private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    glm::mat4 transform;
public:
    RenderComponent(const std::shared_ptr<Entity> &entity, std::shared_ptr<Shader> &shader, std::shared_ptr<Mesh> &mesh);

    void update() override;

    Component *clone(std::shared_ptr<Entity> entity) override;
};


#endif //GAMEENGINE301CR_RENDERCOMPONENT_H
