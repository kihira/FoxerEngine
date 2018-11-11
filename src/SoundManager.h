
#ifndef GAMEENGINE301CR_SOUNDMANAGER_H
#define GAMEENGINE301CR_SOUNDMANAGER_H

#include <soloud.h>


class SoundManager {
private:
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

    void update();
};


#endif //GAMEENGINE301CR_SOUNDMANAGER_H
