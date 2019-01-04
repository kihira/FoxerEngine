
#ifndef GAMEENGINE301CR_NETWORKCOMPONENT_H
#define GAMEENGINE301CR_NETWORKCOMPONENT_H


#include <glm/vec3.hpp>
#include "../entity/Component.h"

/**
 * Component for synchronising physics and position across the network
 */
class NetworkComponent : Component {
private:
    unsigned short clientAuthority; // The client ID that has authority
    bool isAuthoritive; // Whether this component has authority over the entity its attached to

    float syncRate; // How often to update in seconds
    float lastSyncTime;
    glm::vec3 lastPosition;
    glm::vec3 lastRotation;
public:
    NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority);

    void update(float deltaTime) override;

    Component *clone(std::shared_ptr<Entity> entity) override;

    bool hasAuthority() const;
};


#endif //GAMEENGINE301CR_NETWORKCOMPONENT_H
