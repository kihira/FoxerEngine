
#include "PhysicsComponent.h"
#include "../Managers.h"

void PhysicsComponent::update() {
    glm::vec3 pos;
    pos.x = body->GetPosition().x;
    pos.z = body->GetPosition().y;
    entity->setPosition(pos);
}

PhysicsComponent::PhysicsComponent(const std::shared_ptr<Entity> &entity) : entity(entity) {
    b2BodyDef bodyDef;
    bodyDef.userData = reinterpret_cast<void *>(entity.get());
    bodyDef.position.Set(entity->getPosition().x, entity->getPosition().z);
    bodyDef.angle = entity->getRotation().y;
    bodyDef.type = b2BodyType::b2_dynamicBody; // todo this should change
    body = gPhysicsManager.createBody(bodyDef);
}
