
#include "NetworkComponent.h"
#include <glm/glm.hpp>
#include "../entity/Entity.h"
#include "../Managers.h"
#include "NetworkManager.h"
#include "../physics/PhysicsComponent.h"

void NetworkComponent::update() {
}

Component *NetworkComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new NetworkComponent(entity, isAuthoritive);
    return newComponent;
}

NetworkComponent::NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority) : Component(entity),
                                                                                               isAuthoritive(
                                                                                                       hasAuthority) {}

bool NetworkComponent::hasAuthority() const {
    return isAuthoritive;
}
