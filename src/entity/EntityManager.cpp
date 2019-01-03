
#include <easy/profiler.h>
#include "EntityManager.h"
#include "../Managers.h"
#include "../network/NetworkManager.h"
#include "../util/assert.h"
#include "../AssetManager.h"
#include "../event/EventManager.h"


EntityManager::EntityManager() = default; // noop

EntityManager::~EntityManager() = default; // noop

void EntityManager::startUp() {
    logger = spdlog::get("main")->clone("entity");
    SPDLOG_LOGGER_TRACE(logger, "Entity Manager Start Up Begin");

    // Register event handler
    gEventManager.registerHandler(SID("EVENT_TYPE_ENTITY_SPAWN"), this);

    SPDLOG_LOGGER_TRACE(logger, "Entity Manager Start Up Complete");
}

void EntityManager::shutDown() {

}

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
        gAssetManager.loadEntityPrototype(prototypeId);
    }

    auto prototype = prototypes[prototypeId];
    ASSERT(prototype != nullptr);
    auto id = getEntityId();
    auto entity = prototype->clone(id);
    ASSERT(entity != nullptr);

    auto event = Event(SID("EVENT_TYPE_ENTITY_SPAWN"));
    event.setArg("prototypeId", prototypeId);
    event.setArg("entityId", entity->getId());
    event.push();

    entities.emplace(id, entity);
    return entity;
}

std::shared_ptr<Entity> EntityManager::spawn(StringId prototypeId, EntityId entityId) {
    if (prototypes.find(prototypeId) == prototypes.end()) {
        gAssetManager.loadEntityPrototype(prototypeId);
    }

    auto prototype = prototypes[prototypeId];
    ASSERT(prototype != nullptr);
    auto entity = prototype->clone(entityId);
    ASSERT(entity != nullptr);

    entities.emplace(entityId, entity);
    return entity;
}

inline EntityId EntityManager::getEntityId() {
    auto id = static_cast<EntityId>(rand() % USHRT_MAX);
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

std::shared_ptr<Entity> EntityManager::getEntity(EntityId id) {
    if (entities.find(id) == entities.end()) {
        return nullptr;
    }
    return entities[id];
}

bool EntityManager::onEvent(Event &event) {
    switch (event.getType()) {
        // Entity has been spawned on server, time for us to spawn it clientside too
        case SID("EVENT_TYPE_ENTITY_SPAWN"): {
            if (!event.getArg<bool>("fromServer")) return false;
            spawn(event.getArg<StringId>("prototypeId"), event.getArg<EntityId>("entityId"));
        }
    }
    return false;
}
