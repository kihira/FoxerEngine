
#include "PhysicsComponent.h"
#include "../assert.h"

PhysicsComponent::PhysicsComponent(const std::shared_ptr<Entity> &entity, b2Body *body, b2Fixture *fixture) : Component(
        entity), body(body), fixture(fixture) {
    ASSERT(body != nullptr);
    ASSERT(fixture != nullptr);
}

void PhysicsComponent::update() {
    glm::vec3 pos;
    pos.x = body->GetPosition().x;
    pos.z = body->GetPosition().y;
    entity->setPosition(pos);
}

void PhysicsComponent::setUserData(void *data) {
    body->SetUserData(data);
}

void PhysicsComponent::setLinearDamping(float damping) {
    body->SetLinearDamping(damping);
}

void PhysicsComponent::setAngularDamping(float damping) {
    body->SetAngularDamping(damping);
}

void PhysicsComponent::setGravityScale(float scale) {
    body->SetGravityScale(scale);
}

void PhysicsComponent::setActive(bool active) {
    body->SetActive(active);
}

