
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
    // Unload existing level
    if (activeLevel != nullptr) {
        auto event = Event(SID("EVENT_TYPE_LEVEL_UNLOAD"));
        event.setArg("levelId", level);
        event.push();
    }

    // Load new level
    // activeLevel = gAssetManager.loadLevel();

    // Post event
    auto event = Event(SID("EVENT_TYPE_LEVEL_LOAD"));
    event.setArg("levelId", level);
    event.push();
}
