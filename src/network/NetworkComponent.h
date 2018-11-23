
#ifndef GAMEENGINE301CR_NETWORKCOMPONENT_H
#define GAMEENGINE301CR_NETWORKCOMPONENT_H


#include "../entity/Component.h"

class NetworkComponent : Component {
private:
    bool hasAuthority; // Whether this component has authority over the entity its attached to
public:
    void update() override;

private:
    Component *clone(std::shared_ptr<Entity> entity) override;
};


#endif //GAMEENGINE301CR_NETWORKCOMPONENT_H
