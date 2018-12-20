#ifndef GAMEENGINE301CR_ENTITY_H
#define GAMEENGINE301CR_ENTITY_H

#include <glm/vec3.hpp>
#include <sol.hpp>
#include <typeindex>
#include <map>
#include "Component.h"
#include "../event/EventHandler.h"

#define ENTITY_ID unsigned short

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

    const ENTITY_ID id;
    std::string name;
    std::map<std::type_index, Component *> components;
public:
    Entity(ENTITY_ID id, std::string name);

    void update();

    /**
     * Creates a copy of the entity. This should not be called directly unless you are EntityManager
     *
     * Used as part of the prototype pattern
     */
    std::shared_ptr<Entity> clone(ENTITY_ID id);

    bool onEvent(Event &event) override;

    const glm::vec3 &getPosition() const;

    void setPosition(const glm::vec3 &position);

    const glm::vec3 &getRotation() const;

    const std::string &getName() const;

    void setRotation(const glm::vec3 &rotation);

    void setUpdateFn(const sol::protected_function &updateFn);

    void setOnSpawnFn(const sol::function &onSpawnFn);

    void setName(const std::string &name);

    const glm::vec3 &getPrevPosition() const;

    const glm::vec3 &getPrevRotation() const;

    const ENTITY_ID getId() const;

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
