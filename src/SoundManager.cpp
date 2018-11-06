
#include "SoundManager.h"

void SoundManager::startUp() {
    soloud.init();
}

void SoundManager::shutDown() {
    soloud.deinit();
}
