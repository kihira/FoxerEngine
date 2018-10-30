
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

    void startUp();

    void shutDown();

    void update();

    std::shared_ptr<Entity> spawn(std::string name);

    /**
     * This is the callback for when the NetworkManager receives an entity update packet
     * @param data
     * @param dataLength
     */
    void handleEntityPacket(int packetID, void *data, size_t dataLength);
};


#endif //GAMEENGINE301CR_ENTITYMANAGER_H
