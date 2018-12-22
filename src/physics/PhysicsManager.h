
#ifndef GAMEENGINE301CR_PHYSICSMANAGER_H
#define GAMEENGINE301CR_PHYSICSMANAGER_H

#include <Box2D/Box2D.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>
#include "PhysicsComponent.h"


class PhysicsManager : public b2ContactListener, public EventHandler {
private:
    std::shared_ptr<spdlog::logger> logger;
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;
    const float32 timeStep = 1.f/60.f;
    b2World *world; // A pointer is used so we can create a default constructor
    std::vector<PhysicsComponent *> components;
public:
    PhysicsManager();

    ~PhysicsManager() override;

    /**
     * Initialises the PhysicsManager
     */
    void startUp();

    /**
     * De-initialises the PhysicsManager
     */
    void shutDown();

    void update();

    /**
     * Creates a physics body
     * @param bodyDef The body definition
     * @return A pointer to the created body
     */
    b2Body *createBody(const b2BodyDef &bodyDef);

    /**
     * Creates a joint
     * @param jointDef The joint definition
     * @return A pointer to the created joint
     */
    b2Joint *createJoint(const b2JointDef &jointDef);

    void addPhysicsComponent(PhysicsComponent *component);

    void BeginContact(b2Contact *contact) override;

    void EndContact(b2Contact *contact) override;

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_PHYSICSMANAGER_H
