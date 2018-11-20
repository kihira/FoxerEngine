
#include "SoundManager.h"

SoundManager::SoundManager() = default; // noop

SoundManager::~SoundManager() = default; // noop

void SoundManager::startUp() {
    soloud.init();
}

void SoundManager::shutDown() {
    soloud.deinit();
}

void SoundManager::update() {

}
