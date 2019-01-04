
#ifndef GAMEENGINE301CR_SOUNDMANAGER_H
#define GAMEENGINE301CR_SOUNDMANAGER_H

#include <soloud.h>
#include <spdlog/spdlog.h>
#include "../util/StringId.h"


class SoundManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    SoLoud::Soloud soloud;
public:
    SoundManager();

    ~SoundManager();

    /**
     * Initialises the SoundManager
     */
    void startUp();

    /**
     * De-initialises the SoundManager
     */
    void shutDown();

    void update(float deltaTime);
};


#endif //GAMEENGINE301CR_SOUNDMANAGER_H
