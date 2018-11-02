
#include "PhysicsManager.h"

void PhysicsManager::startUp() {
    b2Vec2 gravity(0.f, -10.f);
    world = new b2World(gravity);
}

void PhysicsManager::shutDown() {
    delete world;
}

void PhysicsManager::update() {
    world->Step(timeStep, velocityIterations, positionIterations);
}

b2Body *PhysicsManager::createBody(const b2BodyDef &bodyDef) {
    // Box2D will automatically cleanup memory from these pointers when world goes out of scope
    return world->CreateBody(&bodyDef);
}

b2Joint *PhysicsManager::createJoint(const b2JointDef &jointDef) {
    return world->CreateJoint(&jointDef);
}

PhysicsManager::PhysicsManager() = default;
