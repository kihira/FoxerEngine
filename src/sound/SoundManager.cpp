
#include "SoundManager.h"

SoundManager::SoundManager() = default; // noop

SoundManager::~SoundManager() = default; // noop

void SoundManager::startUp() {
    logger = spdlog::get("main")->clone("sound");
    if (!soloud.init()) {
        logger->error("Failed to initialise SoLoud");
    }
}

void SoundManager::shutDown() {
    soloud.deinit();
}

void SoundManager::update() {

}
