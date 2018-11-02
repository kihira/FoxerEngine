
#ifndef GAMEENGINE301CR_PHYSICSCOMPONENT_H
#define GAMEENGINE301CR_PHYSICSCOMPONENT_H

#include <memory>
#include <Box2D/Dynamics/b2Body.h>
#include "../entity/Entity.h"
#include "../entity/Component.h"


/**
 * A component that binds an entity to a physics body.
 * This is used to help provide a level of abstraction between the entity and physics subsystem
 */
class PhysicsComponent : Component {
private:
    std::shared_ptr<Entity> entity;
    b2Body *body;
public:
    PhysicsComponent(const std::shared_ptr<Entity> &entity);

    void update() override;
};


#endif //GAMEENGINE301CR_PHYSICSCOMPONENT_H
