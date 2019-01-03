#ifndef GAMEENGINE301CR_PLAYERCOMPONENT_H
#define GAMEENGINE301CR_PLAYERCOMPONENT_H


#include "entity/Component.h"

/**
 * A component that can be attached to entities that are controllable by the player
 */
class PlayerComponent : public Component {
private:
    bool isLocalPlayer;
    unsigned short controllerClientId;
public:
    explicit PlayerComponent(const std::shared_ptr<Entity> &entity);

    virtual ~PlayerComponent();

    void update() override;

    Component *clone(std::shared_ptr<Entity> entity) override;
};


#endif //GAMEENGINE301CR_PLAYERCOMPONENT_H
