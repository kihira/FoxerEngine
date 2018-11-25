
#include "PhysicsComponent.h"
#include "../assert.h"
#include "../Managers.h"

PhysicsComponent::PhysicsComponent(const std::shared_ptr<Entity> &entity, b2BodyDef &bodyDef,
                                   b2FixtureDef &fixtureDef) : Component(entity), bodyDef(bodyDef), fixtureDef(fixtureDef) {
    body = gPhysicsManager.createBody(bodyDef);
    fixture = body->CreateFixture(&this->fixtureDef);
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

Component *PhysicsComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new PhysicsComponent(entity, bodyDef, fixtureDef);
    return newComponent;
}

