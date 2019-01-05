
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

	// Used so we don't redundately set position during update loop
	bool ignoreEvent = false;

    // Lua bindings
    sol::function beginContactFn;
    sol::function endContactFn;
public:
    PhysicsComponent(const std::shared_ptr<Entity> &entity, b2BodyDef &bodyDef, b2FixtureDef &fixtureDef,
                         sol::function &beginContactFn, sol::function &endContactFn);

    void update(float deltaTime) override;

    Component *clone(std::shared_ptr<Entity> entity) override;

    void setActive(bool active) override;

    void setLinearDamping(float damping);

    void setAngularDamping(float damping);

    void setGravityScale(float scale);

    glm::vec2 getVelocity() const;

    void setVelocity(glm::vec2 &velocity);

    const glm::vec2 getPosition() const;

    const float getRotation() const;

	/**
	 * Sets the position and rotation for the physics body
	 * Does not immediately update the entities position and rotation.
	 * It is preferred to set the entity position and rotation instead of using this function
	 */
    void setPositionAndRotation(glm::vec2 &position, float rotation);

    /**
     * Sets the angular velocity for the body
     * @param velocity
     */
    void setAngularVelocity(float velocity);

    const float getAngularVelocity() const;

    void beginContact(PhysicsComponent *other);

    void endContact(PhysicsComponent *other);

    bool onEvent(Event& event) override;
};


#endif //GAMEENGINE301CR_PHYSICSCOMPONENT_H
