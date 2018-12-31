
#include "LevelManager.h"
#include "../Managers.h"
#include "../AssetManager.h"
#include "../event/EventManager.h"

LevelManager::LevelManager() = default;

LevelManager::~LevelManager() = default;

void LevelManager::startUp() {
    logger = spdlog::stdout_color_mt("level");
}

void LevelManager::shutDown() {

}

void LevelManager::loadLevel(const std::string &levelName) {
    // Unload existing level
//    if (activeLevel != nullptr) {
//        auto event = Event(SID("EVENT_TYPE_LEVEL_UNLOAD"));
//        event.setArg("levelId", level);
//        event.push();
//    }

    // Load new level
    logger->info("Loading level {}", levelName);
    activeLevel = gAssetManager.loadLevel(levelName);

    // Register its events
    if (activeLevel->canHandleEvents()) {
        gEventManager.registerHandler(activeLevel->getEvents(), activeLevel.get());
    }

    // Post event
    auto event = Event(SID("EVENT_TYPE_LEVEL_LOAD"));
    event.push();
}

void LevelManager::update() {
    if (activeLevel != nullptr) {
        activeLevel->update();
    }
}
