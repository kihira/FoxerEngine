
#ifndef GAMEENGINE301CR_PHYSICSMANAGER_H
#define GAMEENGINE301CR_PHYSICSMANAGER_H

#include <Box2D/Box2D.h>
#include <vector>
#include "PhysicsComponent.h"


class PhysicsManager {
private:
    const int32 velocityIterations = 6;
    const int32 positionIterations = 2;
    const float32 timeStep = 1.f/60.f;
    b2World *world; // A pointer is used so we can create a default constructor
    std::vector<PhysicsComponent *> components;
public:
    PhysicsManager();

    ~PhysicsManager();

    void startUp();

    void shutDown();

    void update();

    b2Body *createBody(const b2BodyDef &bodyDef);

    b2Joint *createJoint(const b2JointDef &jointDef);

    PhysicsComponent *createComponent(b2BodyType type, glm::vec3 position = glm::vec3(0.f), glm::vec3 rotation = glm::vec3(0.f), bool bullet = false, bool fixedRotation = false);

    void deregisterComponent(PhysicsComponent *component);
};


#endif //GAMEENGINE301CR_PHYSICSMANAGER_H
