
#include <iostream>
#include <fmod_errors.h>
#include "SoundManager.h"

SoundManager::SoundManager() {
    result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->init(512, FMOD_INIT_NORMAL, nullptr);
    ERRCHECK(result);

    uint version;
    system->getVersion(&version);
    ERRCHECK(result);
    std::cout << "FMOD " << version << std::endl;
}

SoundManager::~SoundManager() {
    for (auto &sound : sounds) {
        sound->release();
        delete sound;
    }

    system->close();
    system->release();
    delete system;
}


void SoundManager::ERRCHECK_fn(FMOD_RESULT result, const char *file, int line) {
    if (result == FMOD_OK) return;

    std::cerr << "FMOD Error " << result << ":" << std::endl
              << FMOD_ErrorString(result) << std::endl
              << "" << file << ":" << line << std::endl;
    exit(-1);
}

FMOD::Sound *SoundManager::createSound(const char *file) {
    FMOD::Sound *sound;

    system->createSound(file, FMOD_DEFAULT, nullptr, &sound);
    ERRCHECK(result);

    sounds.push_back(sound);
    return sound;
}
