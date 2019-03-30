#include <memory>
#include "../util/assert.h"
#include "../event/EventHandler.h"


#ifndef FOXERENGINE_COMPONENT_H
#define FOXERENGINE_COMPONENT_H


class Entity; // Forward deceleration
class Event;

class Component : public EventHandler {
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

	bool onEvent(Event &event) override { return false; }

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


#endif //FOXERENGINE_COMPONENT_H
