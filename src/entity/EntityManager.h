
#ifndef GAMEENGINE301CR_ENTITYMANAGER_H
#define GAMEENGINE301CR_ENTITYMANAGER_H


#include <cstddef>
#include "Entity.h"

struct EntityPacketData {
    unsigned short packetID;
    glm::vec3 position;
    glm::vec3 rotation;
};

class EntityManager {
private:
    std::map<std::string, std::shared_ptr<Entity>> prototypes;
    std::map<int, std::shared_ptr<Entity>> entities;

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
    void registerPrototype(std::string id, std::shared_ptr<Entity> prototype);

    /**
     * Spawns an entity from it's registered prototype with that ID
     * @param id The ID of the entity
     * @return The newly created entity
     */
    std::shared_ptr<Entity> spawn(std::string id);

    /**
     * This is the callback for when the NetworkManager receives an entity update packet
     * @param data
     * @param dataLength
     */
    void handleEntityPacket(int packetID, void *data, size_t dataLength);
};


#endif //GAMEENGINE301CR_ENTITYMANAGER_H
