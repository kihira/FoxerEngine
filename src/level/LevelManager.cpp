
#include "LevelManager.h"
#include "../Managers.h"
#include "../AssetManager.h"
#include "../event/EventManager.h"


LevelManager::LevelManager() = default;

LevelManager::~LevelManager() = default;

void LevelManager::startUp() {
    logger = spdlog::get("main")->clone("level");
    spdlog::register_logger(logger);

    gEventManager.registerHandler(SID("EVENT_TYPE_PLAYER_CONNECTED"), this);
}

void LevelManager::shutDown() {

}

void LevelManager::loadLevel(const StringId levelId) {
    // Unload existing level
    if (activeLevel != nullptr) {
        auto event = Event(SID("EVENT_TYPE_LEVEL_UNLOAD"));
        event.setArg("levelId", activeLevel->getId());
        event.push();
        activeLevel = nullptr;
    }

    // Load new level
    logger->info("Loading level {}", levelId);
    activeLevel = gAssetManager.loadLevel(levelId);

    // Register its events
    if (activeLevel->canHandleEvents()) {
        gEventManager.registerHandler(activeLevel->getEvents(), activeLevel.get());
    }

    // Post event
    auto event = Event(SID("EVENT_TYPE_LEVEL_LOAD"));
    event.setArg("levelId", activeLevel->getId());
    event.push();
}

void LevelManager::update() {
    if (activeLevel != nullptr) {
        activeLevel->update();
    }
}

bool LevelManager::onEvent(Event &event) {
    switch (event.getType()) {
        case SID("EVENT_TYPE_PLAYER_CONNECTED"): {
            auto levelLoadEvent = Event(SID("EVENT_TYPE_LEVEL_LOAD_SERVER"));
            levelLoadEvent.setArg("sendToClient", event.getArg<unsigned short>("clientId"));
            levelLoadEvent.setArg("levelId", activeLevel->getId());
            levelLoadEvent.push();
            break;
        }
        case SID("EVENT_TYPE_LEVEL_LOAD_SERVER"): {
            loadLevel(event.getArg<StringId>("levelId"));
            break;
        }
    }
    return false;
}
