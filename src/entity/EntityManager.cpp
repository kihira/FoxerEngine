
#include <glm/glm.hpp>
#include <easy/profiler.h>
#include "../Managers.h"
#include "../assert.h"
#include "EntityManager.h"


EntityManager::EntityManager() = default; // noop

EntityManager::~EntityManager() = default; // noop

void EntityManager::startUp() {
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
        std::cerr << "An entity with the id " << id << " already exists" << std::endl;
        return;
    }

    prototypes.insert(std::make_pair(id, prototype));
}

std::shared_ptr<Entity> EntityManager::spawn(std::string name) {
    if (prototypes.find(name) == prototypes.end()) {
        std::cerr << "Unable to find entity prototype with name: " << name << std::endl;
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
    EntityUpdtePacketData packetData = *(EntityUpdtePacketData *)data;

    if (entities.find(packetData.entityId) == entities.end()) return;
    auto entity = entities[packetData.entityId];
    entity->setPosition(packetData.position);
    entity->setRotation(packetData.rotation);
}

void EntityManager::handleEntitySpawnPacket(int packetID, void *data, size_t dataLength) {
    // todo implement spawn packet
}
