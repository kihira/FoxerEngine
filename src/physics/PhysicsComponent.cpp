
#include "PhysicsComponent.h"

void PhysicsComponent::update() {
    glm::vec3 pos;
    pos.x = body->GetPosition().x;
    pos.z = body->GetPosition().y;
    entity->setPosition(pos);
}

// todo should look into moving this into a factory method
PhysicsComponent::PhysicsComponent(const std::shared_ptr<Entity> &entity) : entity(entity) {
//    b2BodyDef bodyDef;
//    bodyDef.userData = reinterpret_cast<void *>(entity.get());
//    bodyDef.position.Set(entity->getPosition().x, entity->getPosition().z);
//    bodyDef.angle = entity->getRotation().y;
//    bodyDef.type = b2BodyType::b2_kinematicBody;
//    //body = gPhysicsManager.createBody(bodyDef);
//
//    b2PolygonShape shape;
//    shape.SetAsBox(.5f, .5f);
//    fixture = body->CreateFixture(&shape, 80.f);
}

PhysicsComponent::PhysicsComponent(b2Body *body) : body(body) {}

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
