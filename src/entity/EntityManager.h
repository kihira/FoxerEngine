
#ifndef GAMEENGINE301CR_ENTITYMANAGER_H
#define GAMEENGINE301CR_ENTITYMANAGER_H


#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Entity.h"

#define ENTITY_SPAWN_ID 12

struct EntitySpawnPacketData {
    StringId prototypeId;
    ENTITY_ID entityId;
};

class EntityManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<StringId, std::shared_ptr<Entity>> prototypes;
    std::map<ENTITY_ID, std::shared_ptr<Entity>> entities;

    /**
     * Gets a new entity ID that is not currently in use;
     */
    unsigned short getEntityId();
public:
    EntityManager();

    ~EntityManager();

    /**
     * Initialises the EntityManager
     */
    void startUp();

    /**
     * De-initialises the EntityManager
     */
    void shutDown();

    void update();

    /**
     * Registers an entity prototype that can be used when spawning new entities of the same type
     * @param id The ID of the entity that can be used to spawn
     * @param prototype The prototype instance of the entity
     */
    void registerPrototype(StringId id, std::shared_ptr<Entity> prototype);

    /**
     * Spawns an entity from it's registered prototype with that ID
     * @param id The ID of the entity
     * @return The newly created entity
     */
    std::shared_ptr<Entity> spawn(StringId id);

    /**
     * Gets the entity with the specified id.
     * If no entity with the ID exists, returns a nullptr
     * @param id The ID of the entity
     * @return The entity
     */
    std::shared_ptr<Entity> getEntity(ENTITY_ID id);

    /**
     * Callback for when an entity spawn packet is received
     * @param packetID
     * @param data
     * @param dataLength
     */
    void handleEntitySpawnPacket(int packetID, void *data, size_t dataLength);
};


#endif //GAMEENGINE301CR_ENTITYMANAGER_H
