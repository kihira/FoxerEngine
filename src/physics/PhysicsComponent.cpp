
#include "PhysicsComponent.h"
#include "../util/assert.h"
#include "../Managers.h"
#include "PhysicsManager.h"

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

void PhysicsComponent::update(float deltaTime) {
    glm::vec3 pos;
    pos.x = body->GetPosition().x;
    pos.z = body->GetPosition().y;
    entity->setPosition(pos);
}

Component *PhysicsComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new PhysicsComponent(entity, bodyDef, fixtureDef, beginContactFn, endContactFn);
    return newComponent;
}

void PhysicsComponent::setActive(bool active) {
    body->SetActive(active);
    Component::setActive(active);
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

glm::vec2 PhysicsComponent::getVelocity() const {
    auto v = body->GetLinearVelocity();
    return glm::vec2(v.x, v.y);
}

void PhysicsComponent::setVelocity(glm::vec2 &velocity) {
    body->SetLinearVelocity(b2Vec2(velocity.x, velocity.y));
}

void PhysicsComponent::setPositionAndRotation(glm::vec2 &position, float rotation) {
    body->SetTransform(b2Vec2(position.x, position.y), rotation);
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

const glm::vec2 PhysicsComponent::getPosition() const {
    auto pos = body->GetPosition();
    return glm::vec2(pos.x, pos.y);
}

const float PhysicsComponent::getRotation() const {
    return body->GetAngle();
}

void PhysicsComponent::setAngularVelocity(float velocity) {
    body->SetAngularVelocity(velocity);
}

