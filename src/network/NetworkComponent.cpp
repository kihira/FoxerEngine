
#include "NetworkComponent.h"
#include "../entity/Entity.h"
#include "../Managers.h"

void NetworkComponent::update() {
    if (!hasAuthority) return;

    if (entity->getPrevPosition() != entity->getPosition() ||
        entity->getPrevRotation() != entity->getRotation()) {
        EntityUpdatePacketData data = {
                entity->getId(),
                entity->getPosition(),
                entity->getRotation()
        };

        if (gNetworkManager.isServer()) {
            gNetworkManager.sendToAllClients(ENTITY_UPDATE_ID, &data, sizeof(EntityUpdatePacketData));
        } else {
            gNetworkManager.sendToServer(ENTITY_UPDATE_ID, &data, sizeof(EntityUpdatePacketData));
        }
    }
}

Component *NetworkComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new NetworkComponent(entity);
    return newComponent;
}
