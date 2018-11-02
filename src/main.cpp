#include <memory>
#include <iostream>
#include <sol.hpp>
#include "render/RenderManager.h"
#include "KeyHandler.h"
#include "AssetManager.h"
#include "entity/EntityManager.h"
#include "network/NetworkManager.h"
#include "Managers.h"

int main(int argc, char **argv) {
    gAssetManager.startUp();
    gRenderManager.startUp();
    gNetworkManager.startUp();
    gPhysicsManager.startUp();
    gEntityManager.startUp();

    gNetworkManager.registerPacket({0, 0, ENET_PACKET_FLAG_UNSEQUENCED, [](int packetID, void *data, size_t dataLength){ gEntityManager.handleEntityPacket(packetID, data, dataLength); }});

    /*
     * Setup key handler
     */
    // todo
//    auto keyHandler = std::make_unique<KeyHandler>();
//    glfwSetWindowUserPointer(renderManager->getWindow(), keyHandler.get());
//    glfwSetKeyCallback(renderManager->getWindow(), KeyHandler::keyCallback);

    sol::table engineTable = gAssetManager.getLua().create_named_table("engine"); // Namespace for interacting with the engine

    // Load functions for lua
    engineTable.set_function("registerEntityPrototype", [](std::string fileName, std::string tableName) -> std::shared_ptr<Entity> { return gAssetManager.loadEntityPrototype(fileName, tableName); });
    engineTable.set_function("spawnEntity", &EntityManager::spawn, EntityManager::instance());

    // Register vec3 type
    engineTable.new_usertype<glm::vec3>(
            "vec3",
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
            );

    // Register entity type
    engineTable.new_usertype<Entity>(
            "entity",
            // sol::constructors<Entity(const char *)>(),
            "noconstructor", sol::no_constructor, // No constructor as we use factory
            "spawn", [](std::string name){return EntityManager::instance()->spawn(name);}, // Provides a method for retrieving a copy of a prototype
            // Register properties
            "name", sol::property(&Entity::getName, &Entity::setName),
            "position", sol::property(&Entity::getPosition, &Entity::setPosition),
            "rotation", sol::property(&Entity::getRotation, &Entity::setRotation)
            );

    // Register level type
    engineTable.new_usertype<Level>(
            "level",
            "noconstructor", sol::no_constructor,
            "name", sol::property(&Level::getName, &Level::setName)
            );

//    engineTable.set_function("registerKeyHandler", [&keyHandler](sol::function callback) -> void {
//        return keyHandler->registerKeyHandlerLua(callback);
//    });

    // Register entity prototypes
    EntityManager::instance()->registerPrototype("player", gAssetManager.loadEntityPrototype("entity", "testEntity"));

    // Load initial level
    auto level = gAssetManager.loadLevel("level1");

    // Start as server or connect to one
    if (argc == 2) {
        gNetworkManager.startServer();
    } else {
        gNetworkManager.connectToServer("localhost", 1234);
    }

    // Main loop
    while (!gRenderManager.shouldClose()) {
        gRenderManager.frameStart();

        gPhysicsManager.update();
        gNetworkManager.update();
        gEntityManager.update();
        gRenderManager.update();

        gRenderManager.frameEnd();
    }

    // Shutdown subsystems
    gEntityManager.shutDown();
    gPhysicsManager.shutDown();
    gNetworkManager.shutDown();
    gRenderManager.shutDown();
    gAssetManager.shutDown();

    return 0;
}