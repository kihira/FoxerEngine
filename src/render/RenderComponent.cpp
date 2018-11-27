
#include <glm/gtc/matrix_transform.hpp>
#include "RenderComponent.h"
#include "../Managers.h"
#include "../vectors.h"
#include "../entity/Entity.h"
#include "../gl_helper.hpp"

RenderComponent::RenderComponent(const std::shared_ptr<Entity> &entity, std::shared_ptr<Shader> &shader, std::shared_ptr<Mesh> &mesh) :
Component(entity), shader(shader), mesh(mesh) {
    gRenderManager.addRenderComponent(this);
}

void RenderComponent::update() {
    transform = glm::translate(glm::mat4(1.f), entity->getPosition());
    transform = glm::rotate(transform, entity->getRotation().x, vector::RIGHT);
    transform = glm::rotate(transform, entity->getRotation().y, vector::UP);
    transform = glm::rotate(transform, entity->getRotation().z, vector::BACKWARD);
    // transform = glm::scale(transform, scale);

    gRenderManager.useShader(shader);
    shader->setUniform("model", transform);
    mesh->render();
    GLERRCHECK();
}

Component *RenderComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new RenderComponent(entity, shader, mesh);
    return newComponent;
}
