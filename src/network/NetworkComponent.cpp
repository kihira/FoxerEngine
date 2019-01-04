
#include "NetworkComponent.h"
#include <glm/glm.hpp>
#include "../entity/Entity.h"
#include "../Managers.h"
#include "NetworkManager.h"
#include "../physics/PhysicsComponent.h"

void NetworkComponent::update(float deltaTime) {
    lastSyncTime -= deltaTime;

    if (lastSyncTime < 0.f) {
        lastSyncTime = syncRate;

        auto event = Event(SID("MESSAGE_PHYSICS_SYNC"));

        auto position = entity->getPosition();
        event.setArg("posX", position.x);
        event.setArg("posY", position.y);
        event.setArg("posZ", position.z);

        auto rotation = entity->getRotation();
        event.setArg("rotX", rotation.x);
        event.setArg("rotY", rotation.y);
        event.setArg("rotZ", rotation.z);
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
