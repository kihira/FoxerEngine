
#include <glm/gtc/matrix_transform.hpp>
#include "RenderComponent.h"
#include "../Managers.h"
#include "RenderManager.h"
#include "../util/Vectors.h"
#include "../entity/Entity.h"
#include "Mesh.h"
#include "Shader.h"

RenderComponent::RenderComponent(const std::shared_ptr<Entity> &entity, std::shared_ptr<Shader> &shader, std::shared_ptr<Mesh> &mesh) :
Component(entity), shader(shader), mesh(mesh) {
    gRenderManager.addRenderComponent(this);
}

RenderComponent::~RenderComponent() {
	gRenderManager.removeRenderComponent(this);
}

void RenderComponent::update(float deltaTime) {
    transform = glm::translate(glm::mat4(1.f), entity->getPosition());
    transform = glm::rotate(transform, entity->getRotation().x, Vector::RIGHT);
    transform = glm::rotate(transform, entity->getRotation().y, Vector::UP);
    transform = glm::rotate(transform, entity->getRotation().z, Vector::BACKWARD);
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
