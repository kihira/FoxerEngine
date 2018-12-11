
#ifndef GAMEENGINE301CR_PHYSICSCOMPONENT_H
#define GAMEENGINE301CR_PHYSICSCOMPONENT_H

#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include <glm/vec2.hpp>
#include "../entity/Entity.h"


/**
 * A component that binds an entity to a physics body.
 * This is used to help provide a level of abstraction between the entity and physics subsystem
 */
class PhysicsComponent : public Component {
private:
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2Body *body;
    b2Fixture *fixture;

    // Lua bindings
    sol::function beginContactFn;
    sol::function endContactFn;
public:
    PhysicsComponent(const std::shared_ptr<Entity> &entity, b2BodyDef &bodyDef, b2FixtureDef &fixtureDef,
                         sol::function &beginContactFn, sol::function &endContactFn);

    void update() override;

    Component *clone(std::shared_ptr<Entity> entity) override;

    void setActive(bool active) override;

    void setLinearDamping(float damping);

    void setAngularDamping(float damping);

    void setGravityScale(float scale);

    glm::vec2 getVelocity() const;

    void setVelocity(glm::vec2 &velocity);

    const glm::vec2 getPosition() const;

    const float getRotation() const;

    void setPositionAndRotation(glm::vec2 &position, float rotation);

    void beginContact();

    void endContact();
};


#endif //GAMEENGINE301CR_PHYSICSCOMPONENT_H
