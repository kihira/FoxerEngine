
#include "NetworkComponent.h"
#include <glm/glm.hpp>
#include "../entity/Entity.h"
#include "../Managers.h"
#include "NetworkManager.h"
#include "../physics/PhysicsComponent.h"

void NetworkComponent::update(float deltaTime) {
    if (!gNetworkManager.isServer()) return;
    lastSyncTime -= deltaTime;

    if (lastSyncTime < 0.f) {
        lastSyncTime = syncRate;

        auto physics = entity->getComponent<PhysicsComponent>();
        auto event = Event(SID("MESSAGE_PHYSICS_SYNC"));

        auto position = entity->getPosition();
        event.setArg("posX", position.x);
        event.setArg("posZ", position.z);

        auto rotation = entity->getRotation();
        event.setArg("rotX", rotation.x);
        event.setArg("rotZ", rotation.z);

        auto velocity = physics->getVelocity();
        event.setArg("velX", velocity.x);
        event.setArg("velZ", velocity.y);

        auto angular = physics->getAngularVelocity();
        event.setArg("velA", angular);

        event.push();
    }
}

Component *NetworkComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new NetworkComponent(entity, isAuthoritive);
    return newComponent;
}

NetworkComponent::NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority) : Component(entity),
                                                                                               isAuthoritive(
                                                                                                       hasAuthority) {}

bool NetworkComponent::hasAuthority() const {
    return isAuthoritive;
}

void NetworkComponent::setSyncRate(float syncRate) {
    NetworkComponent::syncRate = syncRate;
}
