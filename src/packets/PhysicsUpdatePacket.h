
#ifndef GAMEENGINE301CR_PHYSICSUPDATEPACKET_H
#define GAMEENGINE301CR_PHYSICSUPDATEPACKET_H


#include <glm/vec2.hpp>
#include "../Managers.h"
#include "../entity/Entity.h"
#include "../entity/EntityManager.h"
#include "../physics/PhysicsComponent.h"

#define ENTITY_UPDATE_ID 11

struct PhysicsUpdatePacketData {
    ENTITY_ID entityId;
    glm::vec2 position;
    float rotation;
    glm::vec2 velocity;
};

static void handlePhysicsUpdatePacket(int packetID, void *data, size_t dataLength) {
    PhysicsUpdatePacketData packetData = *(PhysicsUpdatePacketData *)data;
    auto entity = gEntityManager.getEntity(packetData.entityId);

    if (entity == nullptr) {
        spdlog::get("main")->error("Received entity update for {:d} but entity does not exist", packetData.entityId);
        return;
    }

    auto physicsComponent = entity->getComponent<PhysicsComponent>();
    physicsComponent->setPositionAndRotation(packetData.position, packetData.rotation);
    physicsComponent->setVelocity(packetData.velocity);
}

#endif //GAMEENGINE301CR_PHYSICSUPDATEPACKET_H
