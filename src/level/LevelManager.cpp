
#include "LevelManager.h"
#include "../Managers.h"
#include "../AssetManager.h"

LevelManager::LevelManager() = default;

LevelManager::~LevelManager() = default;

void LevelManager::startUp() {

}

void LevelManager::shutDown() {

}

void LevelManager::loadLevel(StringId level) {
    // Unload existing leve
    if (activeLevel != nullptr) {

    }

    // Load new level
    activeLevel = gAssetManager.loadLevel();

    // Post event
    auto event = Event(SID("EVENT_TYPE_LEVEL_LOAD"));
    // event.setArg()
}
