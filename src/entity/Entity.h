#ifndef GAMEENGINE301CR_ENTITY_H
#define GAMEENGINE301CR_ENTITY_H

#include <glm/vec3.hpp>
#include <sol.hpp>
#include <typeindex>
#include <map>
#include "Component.h"
#include "../event/EventHandler.h"

typedef unsigned short EntityId;

class Entity : public EventHandler {
private:
    glm::vec3 position;
    glm::vec3 prevPosition;
    glm::vec3 rotation;
    glm::vec3 prevRotation;
    glm::vec3 scale;
    /*
     * Lua bindings
     */
    sol::function updateFn;
    sol::function onSpawnFn;
    sol::function onEventFn;

    const EntityId id;
    std::string name;
    std::map<std::type_index, Component *> components;
    std::vector<StringId> events;
    std::unordered_map<std::string, sol::object> entries;
public:
    Entity(EntityId id, std::string name);

    virtual ~Entity();

    void update(float deltaTime);

    /**
     * Creates a copy of the entity. This should not be called directly unless you are EntityManager
     *
     * Used as part of the prototype pattern
     */
    std::shared_ptr<Entity> clone(EntityId id);

    bool onEvent(Event &event) override;

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

	/**
	 * Returns the rotation of the entity in degrees
	 */
    const glm::vec3 &getRotation() const;

    const std::string &getName() const;

    void setRotation(const glm::vec3 &rotation);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setOnSpawnFn(const sol::function &onSpawnFn);

    void setOnEventFn(const sol::function &onEventFn);

    void setName(const std::string &name);

    /**
     * Sets the events that this entity will listen for. Used only before an entity is constructed from a prototype
     * @param events The events to listen for
     */
    void setEvents(const std::vector<StringId> &events);

    const glm::vec3 &getPrevPosition() const;

    const glm::vec3 &getPrevRotation() const;

    const EntityId getId() const;

    /**
     * Used by Sol/Lua to dynamically set values on the entity.
     * Allows for a large amount of freedom within scripting
     * @param key The key name
     * @param value The key value
     */
    void dynamicSet(std::string key, sol::stack_object value);

    /**
     * Used by Sol/Lua to dynamically get values on the entity.
     * Allows for a large amount of freedom within scripti
     * @param key The key name
     * @return The value
     */
    sol::object dynamicGet(std::string key);

    void addComponent(std::type_index type, Component *component);

    template <typename T>
    void addComponent(Component *component) {
        components.emplace(std::type_index(typeid(T)), component);
    }

    template <typename T>
    T *getComponent() {
        auto i = components.find(std::type_index(typeid(T)));
        if (i != components.end()) {
            return (T *) i->second;
        }
        return nullptr;
    }
};


#endif //GAMEENGINE301CR_ENTITY_H
