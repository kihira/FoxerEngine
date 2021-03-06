
#ifndef FOXERENGINE_RENDERCOMPONENT_H
#define FOXERENGINE_RENDERCOMPONENT_H


#include "../entity/Component.h"
#include <glm/mat4x4.hpp>

class Shader;
class Mesh;

class RenderComponent : public Component {
private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> mesh;
    glm::mat4 transform;
public:
    RenderComponent(const std::shared_ptr<Entity> &entity, std::shared_ptr<Shader> &shader, std::shared_ptr<Mesh> &mesh);

	~RenderComponent() override;

    void update(float deltaTime) override;

    Component *clone(std::shared_ptr<Entity> entity) override;

    bool onEvent(Event& event) override;

	void updateTransform();
};


#endif //FOXERENGINE_RENDERCOMPONENT_H
