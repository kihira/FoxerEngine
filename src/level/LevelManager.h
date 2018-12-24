
#ifndef GAMEENGINE301CR_LEVELMANAGER_H
#define GAMEENGINE301CR_LEVELMANAGER_H


#include "Level.h"

class LevelManager {
private:
    std::shared_ptr<Level> activeLevel;
public:
    LevelManager();

    virtual ~LevelManager();

    void startUp();

    void shutDown();

    void loadLevel(StringId level);
};


#endif //GAMEENGINE301CR_LEVELMANAGER_H
