
#include "NetworkComponent.h"
#include <glm/glm.hpp>
#include "../entity/Entity.h"
#include "../Managers.h"
#include "../entity/EntityManager.h"
#include "NetworkManager.h"
#include "../physics/PhysicsComponent.h"

void NetworkComponent::update() {
    if (!hasAuthority) return;

    auto physicsComponent = entity->getComponent<PhysicsComponent>();
    if (glm::length(entity->getPrevPosition() - entity->getPosition()) > 0.5f ||
        glm::length(entity->getPrevRotation() - entity->getRotation()) > 0.5f ) {
        EntityUpdatePacketData data = {
                entity->getId(),
                entity->getPosition(),
                entity->getRotation(),
                physicsComponent->getVelocity()
        };

        if (gNetworkManager.isServer()) {
            gNetworkManager.sendToAllClients(ENTITY_UPDATE_ID, &data, sizeof(EntityUpdatePacketData));
        } else {
            gNetworkManager.sendToServer(ENTITY_UPDATE_ID, &data, sizeof(EntityUpdatePacketData));
        }
    }
}

Component *NetworkComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new NetworkComponent(entity, hasAuthority);
    return newComponent;
}

NetworkComponent::NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority) : Component(entity),
                                                                                               hasAuthority(
                                                                                                       hasAuthority) {}
