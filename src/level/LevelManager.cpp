
#include "LevelManager.h"
#include "../Managers.h"
#include "../AssetManager.h"
#include "../event/EventManager.h"
#include "../network/NetworkManager.h"

#define PACKET_ID_LEVEL_LOAD 31

struct LevelInfoPacket {
    StringId levelId;
};

LevelManager::LevelManager() = default;

LevelManager::~LevelManager() = default;

void LevelManager::startUp() {
    logger = spdlog::get("main")->clone("level");
    spdlog::register_logger(logger);

    gEventManager.registerHandler(SID("EVENT_TYPE_PLAYER_CONNECTED"), this);
    // TODO could look into creating an event that indicates a certain event should only be sent to one client etc
    gNetworkManager.registerPacket({
        PACKET_ID_LEVEL_LOAD,
        0,
        ENET_PACKET_FLAG_RELIABLE,
        [](int packetID, void *data, size_t dataLength) {
            StringId levelId = static_cast<LevelInfoPacket *>(data)->levelId;
            gLevelManager.loadLevel(levelId);
        }
    });
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
            // Tell connected client to load currently active level
            LevelInfoPacket data { activeLevel->getId() };
            gNetworkManager.sendToClient(event.getArg<ClientId>("clientId"), PACKET_ID_LEVEL_LOAD, &data, sizeof(LevelInfoPacket));
            return false;
        }
    }
    return false;
}
