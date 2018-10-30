
#include <glm/glm.hpp>
#include "EntityManager.h"

void EntityManager::startUp() {

}

void EntityManager::shutDown() {

}

std::shared_ptr<Entity> EntityManager::spawn(std::string name) {
    if (prototypes.find(name) == prototypes.end()) {
        std::cerr << "Unable to find entity prototype with name: " << name << std::endl;
        return nullptr; // todo return error entity
    }

    auto prototype = prototypes[name];
    auto id = getEntityId();
    auto entity = prototype->clone(id);

    entities.insert(std::make_pair(id, entity));
    return entity;
}

void EntityManager::handleEntityPacket(int packetID, void *data, size_t dataLength) {
    EntityPacketData packetData = *(EntityPacketData *)data;

    if (entities.find(packetData.packetID) == entities.end()) return;
    auto entity = entities[packetData.packetID];
    entity->setPosition(packetData.position);
    entity->setRotation(packetData.rotation);
}

inline unsigned short EntityManager::getEntityId() {
    unsigned short id = 0;
    while (entities.find(id) != entities.end()) {
        id = rand() % USHRT_MAX;
    }
    return id;
}

void EntityManager::update() {

}
