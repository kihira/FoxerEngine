
#include "SoundManager.h"

SoundManager::SoundManager() = default; // noop

SoundManager::~SoundManager() = default; // noop

void SoundManager::startUp() {
    logger = spdlog::stdout_color_st("sound");
    if (!soloud.init()) {
        logger->error("Failed to initialise SoLoud");
    }
}

void SoundManager::shutDown() {
    soloud.deinit();
}

void SoundManager::update() {

}
