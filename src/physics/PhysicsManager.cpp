
#include "PhysicsManager.h"
#include <easy/profiler.h>

void PhysicsManager::startUp() {
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

PhysicsComponent *PhysicsManager::createComponent(b2BodyType type, glm::vec3 position, glm::vec3 rotation, bool bullet, bool fixedRotation) {
    b2BodyDef bodyDef;
    bodyDef.type = type;
    bodyDef.position.Set(position.x, position.z);
    bodyDef.angle = rotation.y;
    bodyDef.bullet = bullet;
    bodyDef.fixedRotation = fixedRotation;

    auto component = new PhysicsComponent(createBody(bodyDef));
    components.push_back(component);
    return component;
}

PhysicsManager::~PhysicsManager() = default;

PhysicsManager::PhysicsManager() = default;
