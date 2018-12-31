
#include "NetworkComponent.h"
#include <glm/glm.hpp>
#include "../entity/Entity.h"
#include "../Managers.h"
#include "NetworkManager.h"
#include "../physics/PhysicsComponent.h"
#include "../packets/PhysicsUpdatePacket.h"

void NetworkComponent::update() {
    if (!isAuthoritive) return;

    auto physicsComponent = entity->getComponent<PhysicsComponent>();
    if (glm::length(entity->getPrevPosition() - entity->getPosition()) > 0.5f ||
        glm::length(entity->getPrevRotation() - entity->getRotation()) > 0.5f ) {
        PhysicsUpdatePacketData data = {
                entity->getId(),
                physicsComponent->getPosition(),
                physicsComponent->getRotation(),
                physicsComponent->getVelocity()
        };

        if (gNetworkManager.isServer()) {
            gNetworkManager.sendToAllClients(ENTITY_UPDATE_ID, &data, sizeof(PhysicsUpdatePacketData));
        } else {
            gNetworkManager.sendToServer(ENTITY_UPDATE_ID, &data, sizeof(PhysicsUpdatePacketData));
        }
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
