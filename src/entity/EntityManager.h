
#ifndef GAMEENGINE301CR_ENTITYMANAGER_H
#define GAMEENGINE301CR_ENTITYMANAGER_H


#include <spdlog/spdlog.h>
#include "Entity.h"

class EntityManager : public EventHandler {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::map<StringId, std::shared_ptr<Entity>> prototypes;
    std::map<EntityId, std::shared_ptr<Entity>> entities;

    /**
     * Gets a new entity ID that is not currently in use;
     */
    EntityId getEntityId();
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

    void update(float deltaTime);

    /**
     * Registers an entity prototype that can be used when spawning new entities of the same type
     * @param id The ID of the entity that can be used to spawn
     * @param prototype The prototype instance of the entity
     */
    void registerPrototype(StringId id, std::shared_ptr<Entity> prototype);

    /**
     * Spawns an entity from it's registered prototype with that ID
     * @param prototypeId The ID of the entity
     * @return The newly created entity
     */
    std::shared_ptr<Entity> spawn(StringId prototypeId);

    /**
     * Spawns an entity with an ID already set.
     * This should be used when you know the ID is not in use (such as when spawning level defined entities)
     * DOES NOT fire the EVENT_TYPE_SPAWN_ENTITY event
     * @param prototypeId ID of the prototype entity
     * @param entityId ID of the entity
     * @return The newly created entity
     */
    std::shared_ptr<Entity> spawn(StringId prototypeId, EntityId entityId);

    /**
     * Gets the entity with the specified id.
     * If no entity with the ID exists, returns a nullptr
     * @param id The ID of the entity
     * @return The entity
     */
    std::shared_ptr<Entity> getEntity(EntityId id);

    /**
     * Destroys an entity
     * @param id Entity ID
     */
    void destroy(EntityId id);

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_ENTITYMANAGER_H
