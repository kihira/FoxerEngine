
#ifndef GAMEENGINE301CR_NETWORKCOMPONENT_H
#define GAMEENGINE301CR_NETWORKCOMPONENT_H


#include "../entity/Component.h"

class NetworkComponent : Component {
private:
    bool hasAuthority; // Whether this component has authority over the entity its attached to
public:
    NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority);

    void update() override;

    Component *clone(std::shared_ptr<Entity> entity) override;
};


#endif //GAMEENGINE301CR_NETWORKCOMPONENT_H
