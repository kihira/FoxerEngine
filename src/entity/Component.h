#include <utility>
#include <memory>
#include "../assert.h"


#ifndef GAMEENGINE301CR_COMPONENT_H
#define GAMEENGINE301CR_COMPONENT_H


class Entity; // Forward deceleration

class Component {
protected:
    std::shared_ptr<Entity> entity;
public:
    explicit Component(std::shared_ptr<Entity> entity) : entity(std::move(entity)) {
        ASSERT(Component::entity != nullptr);
    }

    virtual void update() = 0;

    virtual Component *clone(std::shared_ptr<Entity> entity) = 0;
};


#endif //GAMEENGINE301CR_COMPONENT_H
