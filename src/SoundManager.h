
#ifndef GAMEENGINE301CR_SOUNDMANAGER_H
#define GAMEENGINE301CR_SOUNDMANAGER_H

#include <soloud.h>


class SoundManager {
private:
    SoLoud::Soloud soloud;
public:
    void startUp();
    void shutDown();
    void update();
};


#endif //GAMEENGINE301CR_SOUNDMANAGER_H
