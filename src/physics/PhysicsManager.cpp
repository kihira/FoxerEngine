
#include "PhysicsManager.h"
#include "../Managers.h"
#include <easy/profiler.h>
#include "../event/EventManager.h"

PhysicsManager::PhysicsManager() = default; // noop

PhysicsManager::~PhysicsManager() = default; // noop

void PhysicsManager::startUp() {
    logger = spdlog::get("main")->clone("physics");

    b2Vec2 gravity(0.f, 0.f);
    world = new b2World(gravity);
    world->SetContactListener(this);

    gEventManager.registerHandler({SID("EVENT_TYPE_ENTITY_SET_POSITION"), SID("EVENT_TYPE_ENTITY_SET_ROTATION")}, this);

    auto event = Event(SID("EVENT_TYPE_PHYSICS_STARTUP"));
    event.push();
}

void PhysicsManager::shutDown() {
    delete world;
}

void PhysicsManager::update(float deltaTime) {
    EASY_FUNCTION(profiler::colors::Magenta);
    world->Step(timeStep, velocityIterations, positionIterations);

    for (auto component : components) {
        if (!component->isActive()) continue;
        component->update(deltaTime);
    }
}

b2Body *PhysicsManager::createBody(const b2BodyDef &bodyDef) {
    // Box2D will automatically cleanup memory from these pointers when world goes out of scope
    return world->CreateBody(&bodyDef);
}

b2Joint *PhysicsManager::createJoint(const b2JointDef &jointDef) {
    return world->CreateJoint(&jointDef);
}

void PhysicsManager::addPhysicsComponent(PhysicsComponent *component) {
    components.emplace_back(component);
}

void PhysicsManager::setGravity(const glm::vec2 &gravity) {
    world->SetGravity(b2Vec2(gravity.x, gravity.y));
}

void PhysicsManager::BeginContact(b2Contact *contact) {
    auto componentA = static_cast<PhysicsComponent *>(contact->GetFixtureA()->GetUserData());
    auto componentB = static_cast<PhysicsComponent *>(contact->GetFixtureB()->GetUserData());

    componentA->beginContact(componentB);
    componentB->beginContact(componentA);
}

void PhysicsManager::EndContact(b2Contact *contact) {
    auto componentA = static_cast<PhysicsComponent *>(contact->GetFixtureA()->GetUserData());
    auto componentB = static_cast<PhysicsComponent *>(contact->GetFixtureB()->GetUserData());

    componentA->endContact(componentB);
    componentB->endContact(componentA);
}

bool PhysicsManager::onEvent(Event &event) {
    return false;
}

const glm::vec2 PhysicsManager::getGravity() const {
    auto gravity = world->GetGravity();
    return glm::vec2(gravity.x, gravity.y);
}
