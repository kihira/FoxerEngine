
#include "NetworkComponent.h"
#include <glm/glm.hpp>
#include "../entity/Entity.h"
#include "../Managers.h"
#include "NetworkManager.h"
#include "../physics/PhysicsComponent.h"
#include "../event/EventManager.h"

NetworkComponent::NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority) : Component(entity), isAuthoritive(hasAuthority) {
    gNetworkManager.addNetworkComponent(this);
}

NetworkComponent::NetworkComponent(const std::shared_ptr<Entity> &entity, float syncRate) : Component(entity), syncRate(syncRate) {
    gNetworkManager.addNetworkComponent(this);
}

NetworkComponent::~NetworkComponent() {
    gNetworkManager.removeNetworkComponent(this);
    gEventManager.deregisterHandler(SID("MESSAGE_PHYSICS_SYNC"), this);
}

void NetworkComponent::update(float deltaTime) {
    if (!gNetworkManager.isServer()) return;
    lastSyncTime -= deltaTime;

    if (lastSyncTime < 0.f) {
        lastSyncTime = syncRate;

        auto physics = entity->getComponent<PhysicsComponent>();
        auto event = Event(SID("MESSAGE_PHYSICS_SYNC"));
        event.setArg("entityId", entity->getId());

        auto position = entity->getPosition();
        event.setArg("posX", position.x);
		event.setArg("posY", position.y);
        event.setArg("posZ", position.z);

        auto rotation = entity->getRotation();
        event.setArg("rotX", rotation.x);
		event.setArg("rotY", rotation.y);
        event.setArg("rotZ", rotation.z);

        auto velocity = physics->getVelocity();
        event.setArg("velX", velocity.x);
        event.setArg("velZ", velocity.y);

        auto angular = physics->getAngularVelocity();
        event.setArg("velA", angular);

        event.push();
    }
}

Component *NetworkComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new NetworkComponent(entity, syncRate);
    if (!gNetworkManager.isServer()) {
        gEventManager.registerHandler(SID("MESSAGE_PHYSICS_SYNC"), newComponent);
    }
    return newComponent;
}

bool NetworkComponent::hasAuthority() const {
    return isAuthoritive;
}

void NetworkComponent::setSyncRate(float syncRate) {
    NetworkComponent::syncRate = syncRate;
}

bool NetworkComponent::onEvent(Event &event) {
    switch (event.getType()) {
        case SID("MESSAGE_PHYSICS_SYNC"): {
            if (entity->getId() != event.getArg<EntityId>("entityId")) break;

            // Parse event
            auto position = glm::vec3(event.getArg<float>("posX"), event.getArg<float>("posY"), event.getArg<float>("posZ"));
            auto velocity = glm::vec2(event.getArg<float>("velX"), event.getArg<float>("velZ"));
            auto rotation = glm::vec3(event.getArg<float>("rotX"), event.getArg<float>("rotY"), event.getArg<float>("rotZ"));
            auto angular = event.getArg<float>("velA");

            auto physics = entity->getComponent<PhysicsComponent>();
			entity->setPosition(position);
			entity->setRotation(rotation);
            physics->setVelocity(velocity);
            physics->setAngularVelocity(angular);
            break;
        }
    }
    return false;
}
