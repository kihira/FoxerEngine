
#include "PhysicsManager.h"
#include <easy/profiler.h>

PhysicsManager::PhysicsManager() = default; // noop

PhysicsManager::~PhysicsManager() = default; // noop

void PhysicsManager::startUp() {
    logger = spdlog::stdout_color_mt("physics");

    b2Vec2 gravity(0.f, -10.f);
    world = new b2World(gravity);
}

void PhysicsManager::shutDown() {
    delete world;
}

void PhysicsManager::update() {
    EASY_FUNCTION(profiler::colors::Magenta);
    world->Step(timeStep, velocityIterations, positionIterations);
}

b2Body *PhysicsManager::createBody(const b2BodyDef &bodyDef) {
    // Box2D will automatically cleanup memory from these pointers when world goes out of scope
    return world->CreateBody(&bodyDef);
}

b2Joint *PhysicsManager::createJoint(const b2JointDef &jointDef) {
    return world->CreateJoint(&jointDef);
}

void PhysicsManager::deregisterComponent(PhysicsComponent *component) {
    //todo components.erase(components.find(component));
}
