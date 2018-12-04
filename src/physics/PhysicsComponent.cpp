
#include "PhysicsComponent.h"
#include "../assert.h"
#include "../Managers.h"

PhysicsComponent::PhysicsComponent(
        const std::shared_ptr<Entity> &entity,
        b2BodyDef &bodyDef,
        b2FixtureDef &fixtureDef,
        sol::function &beginContactFn,
        sol::function &endContactFn)
        : Component(entity), bodyDef(bodyDef), fixtureDef(fixtureDef), beginContactFn(beginContactFn),
          endContactFn(endContactFn) {
    body = gPhysicsManager.createBody(bodyDef);
    fixture = body->CreateFixture(&this->fixtureDef);

    body->SetUserData(this);
    fixture->SetUserData(this);

    gPhysicsManager.addPhysicsComponent(this);
}

void PhysicsComponent::update() {
    glm::vec3 pos;
    pos.x = body->GetPosition().x;
    pos.z = body->GetPosition().y;
    entity->setPosition(pos);
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
    Component::setActive(active);
}

Component *PhysicsComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new PhysicsComponent(entity, bodyDef, fixtureDef, beginContactFn, endContactFn);
    return newComponent;
}

void PhysicsComponent::setVelocity(glm::vec2 &velocity) {
    b2Vec2 v(velocity.x, velocity.y);
    body->SetLinearVelocity(v);
}

void PhysicsComponent::beginContact() {
    if (beginContactFn != sol::lua_nil) {
        beginContactFn();
    }
}

void PhysicsComponent::endContact() {
    if (endContactFn != sol::lua_nil) {
        endContactFn();
    }
}

