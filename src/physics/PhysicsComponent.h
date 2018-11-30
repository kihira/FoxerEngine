
#ifndef GAMEENGINE301CR_PHYSICSCOMPONENT_H
#define GAMEENGINE301CR_PHYSICSCOMPONENT_H

#include <memory>
#include <Box2D/Dynamics/b2Body.h>
#include <Box2D/Dynamics/b2Fixture.h>
#include "../entity/Entity.h"
#include "../entity/Component.h"


/**
 * A component that binds an entity to a physics body.
 * This is used to help provide a level of abstraction between the entity and physics subsystem
 */
 // todo possible future idea is to save bodyDef to binary as it's just a struct, allowing serialising it
class PhysicsComponent : public Component {
private:
    b2BodyDef bodyDef;
    b2FixtureDef fixtureDef;
    b2Body *body;
    b2Fixture *fixture;
public:
    PhysicsComponent(const std::shared_ptr<Entity> &entity, b2BodyDef &bodyDef, b2FixtureDef &fixtureDef);

    void update() override;

    Component *clone(std::shared_ptr<Entity> entity) override;

    void setUserData(void *data);

    void setLinearDamping(float damping);

    void setAngularDamping(float damping);

    void setGravityScale(float scale);

    void setActive(bool active);

    void applyVelocity(glm::vec2 &velocity);
};


#endif //GAMEENGINE301CR_PHYSICSCOMPONENT_H
