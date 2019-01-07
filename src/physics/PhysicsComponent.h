
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
	glm::vec2 offset = glm::vec2(0.f); // The offset of the physics body from the entities position

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

	/**
	 * Sets whether this component is active or not
	 */
    void setActive(bool active) override;

	/*
	 * Sets the linear damping for the physics body
	 */
    void setLinearDamping(float damping);

	/*
	 * Sets the angular damping for the physics body
	 */
    void setAngularDamping(float damping);

	/**
	 * Sets the gravity scale for the physics body
	 */
    void setGravityScale(float scale);

	/*
	 * Gets the velocity for the physics body
	 */
    glm::vec2 getVelocity() const;

	/*
	 * Sets the velocity for the physics body
	 */
    void setVelocity(glm::vec2 &velocity);

	/*
	 * Gets the position of the physics body.
	 * NOTE: The position of the body may not be the same as the entity as the physics body could be offset
	 */
    const glm::vec2 getPosition() const;

	/**
	 * Returns the rotation in radians
	 */
    const float getRotation() const;

	/**
	 * Sets the position and rotation for the physics body.
	 * Rotation is in degrees
	 * Does not immediately update the entities position and rotation.
	 * It is preferred to set the entity position and rotation instead of using this function
	 * @param position The position
	 * @param rotation The rotation in radians
	 */
    void setPositionAndRotation(glm::vec2 &position, float rotation);

    /**
     * Sets the angular velocity for the body
     * @param velocity
     */
    void setAngularVelocity(float velocity);

	/*
	 * Gets the angular velocity for the physics body in radian/sec
	 */
    float getAngularVelocity() const;

	/**
	 * Sets the body offset relative to the entities position.
	 * This should only be done during the prototype stage.
	 * This does not update the position of the body
	 */
	void setOffset(glm::vec2 offset);

    void beginContact(PhysicsComponent *other);

    void endContact(PhysicsComponent *other);

    bool onEvent(Event& event) override;
};


#endif //GAMEENGINE301CR_PHYSICSCOMPONENT_H
