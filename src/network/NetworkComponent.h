
#ifndef GAMEENGINE301CR_NETWORKCOMPONENT_H
#define GAMEENGINE301CR_NETWORKCOMPONENT_H


#include <glm/vec3.hpp>
#include "../entity/Component.h"
#include "../event/EventHandler.h"

/**
 * Component for synchronising physics and position across the network
 */
class NetworkComponent : public Component, public EventHandler {
private:
    unsigned short clientAuthority; // The client ID that has authority
    bool isAuthoritive; // Whether this component has authority over the entity its attached to

    float syncRate; // How often to update in seconds
    float lastSyncTime;
public:
    NetworkComponent(const std::shared_ptr<Entity> &entity, bool hasAuthority);

    NetworkComponent(const std::shared_ptr<Entity> &entity, float syncRate);

    ~NetworkComponent() override;

    void update(float deltaTime) override;

    Component *clone(std::shared_ptr<Entity> entity) override;

    bool hasAuthority() const;

    /**
     * Sets how often this should send a synchronisation message in seconds
     * @param syncRate
     */
    void setSyncRate(float syncRate);

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_NETWORKCOMPONENT_H
