
#ifndef GAMEENGINE301CR_SOUNDMANAGER_H
#define GAMEENGINE301CR_SOUNDMANAGER_H


#include <vector>
#include <fmod.hpp>

#define ERRCHECK(_result) SoundManager::ERRCHECK_fn(_result, __FILE__, __LINE__)

class SoundManager {
private:
    FMOD::System *system;
    FMOD_RESULT result;
    std::vector<FMOD::Sound *> sounds;
    void ERRCHECK_fn(FMOD_RESULT result, const char *file, int line);
public:
    SoundManager();
    ~SoundManager();
    FMOD::Sound *createSound(const char *file);
};


#endif //GAMEENGINE301CR_SOUNDMANAGER_H
