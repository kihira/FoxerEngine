#include "PhysicsNetworkHandler.h"
#include "../Managers.h"
#include "../network/NetworkManager.h"
#include "../entity/EntityManager.h"
#include "PhysicsComponent.h"
#include "../network/NetworkComponent.h"

#define PACKET_ID_PHYSICS_UPDATE 41

struct PacketPhysicsUpdate {
    EntityId entityId;
    glm::vec2 position;
    glm::vec2 velocity;
    float rotation;
    float angularVelocity;
};

PhysicsNetworkHandler::PhysicsNetworkHandler() {
    gNetworkManager.registerPacket({
        PACKET_ID_PHYSICS_UPDATE,
        0,
        ENET_PACKET_FLAG_RELIABLE,
        [](PacketId packetId, void *data, size_t dataLength) {
            auto packetData = static_cast<PacketPhysicsUpdate *>(data);
            auto entity = gEntityManager.getEntity(packetData->entityId);

            // Verify they have authority
            if (gNetworkManager.isServer()) {
                auto networkComponent = entity->getComponent<NetworkComponent>();
                if (!networkComponent->hasAuthority()) {
                    spdlog::get("network")->warn("Received update packet for entity from client that doesn't have authority");
                    return;
                }
            }

            auto physicsComponent = entity->getComponent<PhysicsComponent>();
            physicsComponent->setPositionAndRotation(packetData->position, packetData->rotation);
            physicsComponent->setVelocity(packetData->velocity);
            physicsComponent->setAngularVelocity(packetData->angularVelocity);
        }
    });
}