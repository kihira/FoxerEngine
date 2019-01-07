
#include "LevelManager.h"
#include "../Managers.h"
#include "../AssetManager.h"
#include "../event/EventManager.h"


LevelManager::LevelManager() = default;

LevelManager::~LevelManager() = default;

void LevelManager::startUp() {
    logger = spdlog::get("main")->clone("level");
    spdlog::register_logger(logger);

    gEventManager.registerHandler({
        SID("EVENT_TYPE_PLAYER_CONNECTED"),
        SID("EVENT_TYPE_LEVEL_LOAD_SERVER")
        }, this);

	// Register lua
	sol::table levelTable = gAssetManager.getLua()["engine"].get<sol::table>().create_named("level");
	levelTable["load"] = [](StringId id) { gLevelManager.loadLevel(id); };
	levelTable["getActive"] = []() -> std::shared_ptr<Level> { return gLevelManager.activeLevel; };

    // Register level type
    levelTable.new_usertype<Level>(
        "level",
        "", sol::no_constructor,
        "name", sol::property(&Level::getName, &Level::setName),
		"id", &Level::getId,
        sol::meta_function::index, &Level::dynamicGet,
        sol::meta_function::new_index, &Level::dynamicSet
    );
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
            if (event.getArg<bool>("fromServer")) break;
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
