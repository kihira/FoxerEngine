
#include <glm/glm.hpp>
#include <easy/profiler.h>
#include <spdlog/spdlog.h>
#include "../Managers.h"
#include "../assert.h"
#include "EntityManager.h"


EntityManager::EntityManager() = default; // noop

EntityManager::~EntityManager() = default; // noop

void EntityManager::startUp() {
    logger = spdlog::stdout_color_mt("entity");

    // Register entity handling packets
    gNetworkManager.registerPacket({ENTITY_UPDATE_ID, 0, ENET_PACKET_FLAG_UNSEQUENCED, [](int packetID, void *data, size_t dataLength){
        gEntityManager.handleEntityUpdatePacket(packetID, data, dataLength);
    }});
    gNetworkManager.registerPacket({ENTITY_SPAWN_ID, 0, ENET_PACKET_FLAG_UNSEQUENCED, [](int packetID, void *data, size_t dataLength){
        gEntityManager.handleEntitySpawnPacket(packetID, data, dataLength);
    }});
}

void EntityManager::shutDown() {

}

// todo it's a little confusing having individual entities using the term id and also id per prototype...
void EntityManager::registerPrototype(std::string id, std::shared_ptr<Entity> prototype) {
    if (prototypes.find(id) != prototypes.end()) {
        logger->error("An entity with the id {0:d} already exists", id);
        return;
    }

    prototypes.insert(std::make_pair(id, prototype));
}

std::shared_ptr<Entity> EntityManager::spawn(std::string name) {
    if (prototypes.find(name) == prototypes.end()) {
        logger->error("Unable to find entity prototype with name: {}", name);
        return nullptr; // todo return error entity
    }

    auto prototype = prototypes[name];
    ASSERT(prototype != nullptr);
    auto id = getEntityId();
    auto entity = prototype->clone(id);
    ASSERT(entity != nullptr);

    entities.insert(std::make_pair(id, entity));
    return entity;
}

inline unsigned short EntityManager::getEntityId() {
    unsigned short id = 0;
    while (entities.find(id) != entities.end()) {
        id = rand() % USHRT_MAX;
    }
    return id;
}

void EntityManager::update() {
    EASY_FUNCTION();
    for (const auto &entity : entities) {
        entity.second->update();
    }
}

void EntityManager::handleEntityUpdatePacket(int packetID, void *data, size_t dataLength) {
    EntityUpdatePacketData packetData = *(EntityUpdatePacketData *)data;

    if (entities.find(packetData.entityId) == entities.end()) {
        logger->error("Received entity update for {:d} but entity does not exist", packetData.entityId);
        return;
    }
    auto entity = entities[packetData.entityId];
    entity->setPosition(packetData.position);
    entity->setRotation(packetData.rotation);
}

void EntityManager::handleEntitySpawnPacket(int packetID, void *data, size_t dataLength) {
    if (gNetworkManager.isServer()) return; // Shouldn't process spawn packets on server
    EntitySpawnPacketData packetData = *(EntitySpawnPacketData *)data;

    if (prototypes.find(packetData.prototypeName) == prototypes.end()) {
        logger->error("Attempted to spawn a {} but no prototype was found", packetData.prototypeName);
        return;
    }
    auto entity = prototypes[packetData.prototypeName]->clone(packetData.entityId);
}
