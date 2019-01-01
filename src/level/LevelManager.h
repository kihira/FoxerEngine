
#ifndef GAMEENGINE301CR_LEVELMANAGER_H
#define GAMEENGINE301CR_LEVELMANAGER_H


#include <spdlog/spdlog.h>
#include "Level.h"

class LevelManager : public EventHandler {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<Level> activeLevel;
public:
    LevelManager();

    virtual ~LevelManager();

    void startUp();

    void shutDown();

    void update();

    /**
     * Loads a level with the specified level ID and unloads the existing one
     * @param levelId ID of the level
     */
    void loadLevel(StringId levelId);

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_LEVELMANAGER_H
