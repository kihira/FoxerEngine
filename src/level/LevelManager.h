
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

    void loadLevel(const std::string &levelName);

    bool onEvent(Event &event) override;
};


#endif //GAMEENGINE301CR_LEVELMANAGER_H
