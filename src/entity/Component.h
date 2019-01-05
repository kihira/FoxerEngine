#include <memory>
#include "../util/assert.h"


#ifndef GAMEENGINE301CR_COMPONENT_H
#define GAMEENGINE301CR_COMPONENT_H


class Entity; // Forward deceleration

class Component {
protected:
    std::shared_ptr<Entity> entity;
    bool active = true;
public:
    explicit Component(std::shared_ptr<Entity> entity) : entity(std::move(entity)) {
        ASSERT(Component::entity != nullptr);
    }

    virtual ~Component() = default;

    virtual void update(float deltaTime) = 0;

    virtual Component *clone(std::shared_ptr<Entity> entity) = 0;

    virtual void setActive(bool active) {
        Component::active = active;
    }

    bool isActive() const {
        return active;
    }

    const std::shared_ptr<Entity>& getEntity() const {
	    return entity;
    };
};


#endif //GAMEENGINE301CR_COMPONENT_H
