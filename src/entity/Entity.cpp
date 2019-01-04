#include <utility>

#include <glm/gtc/matrix_transform.hpp>
#include "Entity.h"
#include "../util/Vectors.h"
#include "../Managers.h"
#include "../event/EventManager.h"

Entity::Entity(const unsigned short id, std::string name) : id(id), name(std::move(name)) {}

Entity::~Entity() {
    for (auto component : components) {
        delete component.second;
    }
}


void Entity::update(float deltaTime) {
    if (updateFn != sol::lua_nil) {
        updateFn(this);
    }
}

const glm::vec3 &Entity::getPosition() const {
    return position;
}

void Entity::setPosition(const glm::vec3 &position) {
    Entity::position = position;
}

const glm::vec3 &Entity::getRotation() const {
    return rotation;
}

const std::string &Entity::getName() const {
    return name;
}

void Entity::setRotation(const glm::vec3 &rotation) {
    Entity::rotation = rotation;
}

void Entity::setUpdateFn(const sol::protected_function &updateFn) {
    Entity::updateFn = updateFn;
}

void Entity::setOnSpawnFn(const sol::function &onSpawnFn) {
    Entity::onSpawnFn = onSpawnFn;
}

std::shared_ptr<Entity> Entity::clone(const unsigned short id) {
    auto newEntity = std::make_shared<Entity>(id, name);

    newEntity->setPosition(position);
    newEntity->setRotation(rotation);
    newEntity->setUpdateFn(updateFn);
    newEntity->setOnSpawnFn(onSpawnFn);

    for (auto component : components) {
        auto newComponent = component.second->clone(newEntity);
        ASSERT(newComponent != nullptr);
        newEntity->addComponent(component.first, newComponent);
    }

    if (newEntity->onSpawnFn != sol::lua_nil) {
        newEntity->onSpawnFn(newEntity);
    }

    // Setup events
    if (onEventFn != sol::lua_nil) {
        newEntity->setOnEventFn(onEventFn);
        newEntity->setEvents(events);
        for (auto &eventId : events) {
            gEventManager.registerHandler(eventId, newEntity.get());
        }
    }

    return newEntity;
}

void Entity::setName(const std::string &name) {
    Entity::name = name;
}

const glm::vec3 &Entity::getPrevPosition() const {
    return prevPosition;
}

const glm::vec3 &Entity::getPrevRotation() const {
    return prevRotation;
}

const unsigned short Entity::getId() const {
    return id;
}

void Entity::addComponent(std::type_index type, Component *component) {
    components.emplace(type, component);
}

bool Entity::onEvent(Event &event) {
    if (onEventFn != sol::lua_nil) {
        return onEventFn(this, event);
    }
    return false;
}

void Entity::setOnEventFn(const sol::function &onEventFn) {
    Entity::onEventFn = onEventFn;
}

void Entity::setEvents(const std::vector<StringId> &events) {
    Entity::events = events;
}

void Entity::dynamicSet(std::string key, sol::stack_object value) {
    auto it = entries.find(key);
    if (it == entries.cend()) {
        entries.insert(it, { std::move(key), std::move(value) });
    }
    else {
        std::pair<const std::string, sol::object>& kvp = *it;
        sol::object& entry = kvp.second;
        entry = sol::object(std::move(value));
    }
}

sol::object Entity::dynamicGet(std::string key) {
    auto it = entries.find(key);
    if (it == entries.cend()) {
        return sol::lua_nil;
    }
    return it->second;
}
