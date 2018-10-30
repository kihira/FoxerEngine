
#include <glm/glm.hpp>
#include "EntityManager.h"
#include "assert.h"

void EntityManager::startUp() {

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
    for (auto entity : entities) {
        entity.second->update();
    }
}

std::shared_ptr<EntityManager> EntityManager::instance_;

std::shared_ptr<EntityManager> EntityManager::instance() {
    if (EntityManager::instance_ == nullptr) {
        EntityManager::instance_ = std::make_shared<EntityManager>();
    }
    return EntityManager::instance_;
}
