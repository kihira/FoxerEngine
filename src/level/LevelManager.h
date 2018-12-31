
#ifndef GAMEENGINE301CR_LEVELMANAGER_H
#define GAMEENGINE301CR_LEVELMANAGER_H


#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Level.h"

class LevelManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    std::shared_ptr<Level> activeLevel;
public:
    LevelManager();

    virtual ~LevelManager();

    void startUp();

    void shutDown();

    void loadLevel(const std::string &levelName);
};


#endif //GAMEENGINE301CR_LEVELMANAGER_H
