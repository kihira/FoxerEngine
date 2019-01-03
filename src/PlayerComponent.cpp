#include "PlayerComponent.h"

PlayerComponent::PlayerComponent(const std::shared_ptr<Entity> &entity) : Component(entity) {}

PlayerComponent::~PlayerComponent() = default;

void PlayerComponent::update() {

}

Component *PlayerComponent::clone(std::shared_ptr<Entity> entity) {
    auto newComponent = new PlayerComponent(entity);
    return newComponent;
}
