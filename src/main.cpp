#include <memory>
#include <iostream>
#include <sol.hpp>
#include <easy/profiler.h>
#include "Managers.h"
#include "KeyHandler.h"

int main(int argc, char **argv) {
#ifndef NDEBUG // Enable profiler in debug mode
    EASY_PROFILER_ENABLE;
#endif

    gRenderManager.startUp();
    gNetworkManager.startUp();
    gPhysicsManager.startUp();
    gEntityManager.startUp();
    gSoundManager.startUp();
    gAssetManager.startUp();

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
    engineTable.set_function("spawnEntity", &EntityManager::spawn, gEntityManager);

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
            "spawn", [](std::string name){return gEntityManager.spawn(name);}, // Provides a method for retrieving a copy of a prototype
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

    // Register entity prototypes
    gEntityManager.registerPrototype("dummyTarget", gAssetManager.loadEntityPrototype("dummyTarget", "dummyTarget"));

    // Load initial level
    // auto level = gAssetManager.loadLevel("level1");

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
    gAssetManager.shutDown();
    gSoundManager.startUp();
    gEntityManager.shutDown();
    gPhysicsManager.shutDown();
    gNetworkManager.shutDown();
    gRenderManager.shutDown();

#ifndef NDEBUG // Dump profile data
    profiler::dumpBlocksToFile("./profile_data.prof");
#endif

    return 0;
}