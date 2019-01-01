
#include <easy/profiler.h>
#include <spdlog/spdlog.h>
#include "EntityManager.h"
#include "../Managers.h"
#include "../network/NetworkManager.h"
#include "../event/EventManager.h"
#include "../util/assert.h"


EntityManager::EntityManager() = default; // noop

EntityManager::~EntityManager() = default; // noop

void EntityManager::startUp() {
    logger = spdlog::get("main")->clone("entity");
    SPDLOG_DEBUG("Entity Manager Start Up");

    prototypes = std::map<StringId, std::shared_ptr<Entity>>();

    // Register entity handling packets
    gNetworkManager.registerPacket({ENTITY_SPAWN_ID, 0, ENET_PACKET_FLAG_UNSEQUENCED, [](int packetID, void *data, size_t dataLength){
        gEntityManager.handleEntitySpawnPacket(packetID, data, dataLength);
    }});
    SPDLOG_DEBUG("Entity Manager Start Up Complete");
}

void EntityManager::shutDown() {

}

// todo it's a little confusing having individual entities using the term id and also id per prototype...
void EntityManager::registerPrototype(StringId id, std::shared_ptr<Entity> prototype) {
    ASSERT(prototype != nullptr);
    if (prototypes.find(id) != prototypes.end()) {
        logger->error("An entity with the id {} already exists", id);
        return;
    }
    prototypes.emplace(id, prototype);
}

std::shared_ptr<Entity> EntityManager::spawn(StringId prototypeId) {
    if (prototypes.find(prototypeId) == prototypes.end()) {
        logger->error("Unable to find entity prototype with id: {}", prototypeId);
        return nullptr; // todo return error entity
    }

    auto prototype = prototypes[prototypeId];
    ASSERT(prototype != nullptr);
    auto id = getEntityId();
    auto entity = prototype->clone(id);
    ASSERT(entity != nullptr);

    auto event = Event(SID("EVENT_TYPE_ENTITY_SPAWN"));
    event.setArg("entityId", entity->getId());
    gEventManager.push(event);

    entities.emplace(id, entity);
    return entity;
}

std::shared_ptr<Entity> EntityManager::spawn(StringId prototypeId, EntityId entityId) {
    if (prototypes.find(prototypeId) == prototypes.end()) {
        logger->error("Unable to find entity prototype with id: {}", prototypeId);
        return nullptr; // todo return error entity
    }

    auto prototype = prototypes[prototypeId];
    ASSERT(prototype != nullptr);
    auto entity = prototype->clone(entityId);
    ASSERT(entity != nullptr);

//    auto event = Event(SID("EVENT_TYPE_ENTITY_SPAWN"));
//    event.setArg("entityId", entity->getId());
//    gEventManager.push(event);

    entities.emplace(entityId, entity);
    return entity;
}

inline unsigned short EntityManager::getEntityId() {
    unsigned short id = static_cast<EntityId>(rand() % USHRT_MAX);
    while (entities.find(id) != entities.end()) {
        id = static_cast<EntityId>(rand() % USHRT_MAX);
    }
    return id;
}

void EntityManager::update() {
    EASY_FUNCTION();
    for (const auto &entity : entities) {
        entity.second->update();
    }
}

void EntityManager::handleEntitySpawnPacket(int packetID, void *data, size_t dataLength) {
    if (gNetworkManager.isServer()) return; // Shouldn't process spawn packets on server
    EntitySpawnPacketData packetData = *(EntitySpawnPacketData *)data;

    if (prototypes.find(packetData.prototypeId) == prototypes.end()) {
        logger->error("Attempted to spawn a {} but no prototype was found", packetData.prototypeId);
        return;
    }
    auto entity = prototypes[packetData.prototypeId]->clone(packetData.entityId);
}

std::shared_ptr<Entity> EntityManager::getEntity(EntityId id) {
    if (entities.find(id) == entities.end()) {
        return nullptr;
    }
    return entities[id];
}
