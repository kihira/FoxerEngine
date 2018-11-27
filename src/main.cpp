#include <memory>
#include <iostream>
#include <sol.hpp>
#include <easy/profiler.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Managers.h"
#include "InputManager.h"

int main(int argc, char **argv) {
#ifndef NDEBUG // Enable profiler in debug mode
    EASY_PROFILER_ENABLE;
#endif
    bool server = argc == 2; // Whether we are a server or not

    // Initialise logger
    auto logger = spdlog::stdout_color_mt("main");

    gAssetManager.startUp();
    if (!server) {
        gRenderManager.startUp();
        gInputManager.startUp();
    }
    gNetworkManager.startUp();
    gPhysicsManager.startUp();
    gEntityManager.startUp();
    gSoundManager.startUp();

    sol::table engineTable = gAssetManager.getLua().create_named_table("engine"); // Namespace for interacting with the engine

    // Load functions for lua
    // Can't pass a class instance as the 3rd parameter as it doesn't seem to work with extern
    // todo current way of creating tables seems messy, should see if there is a cleaner way

    // Entity functions
    sol::table entityTable = engineTable.create_named("entity");
    engineTable["entity"]["registerEntityPrototype"] = [](std::string fileName, std::string tableName) -> std::shared_ptr<Entity> { return gAssetManager.loadEntityPrototype(fileName, tableName); };
    engineTable["entity"]["spawnEntity"] = [](std::string name) -> std::shared_ptr<Entity> { return gEntityManager.spawn(name); };

    // Input functions
    engineTable["input"] = engineTable.create();
    engineTable["input"]["registerKeyHandler"] = [](sol::function handler) { gInputManager.registerKeyHandlerLua(handler); };

    // Register vec3 type
    engineTable.new_usertype<glm::vec3>(
            "vec3",
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z
            );

    // Register entity type
    entityTable.new_usertype<Entity>(
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
    gAssetManager.loadEntityPrototype("dummyTarget", "dummyTarget");

    // Load initial level
    auto level = gAssetManager.loadLevel(gAssetManager.loadSettings()->initialLevel);

    // Start as server or connect to one
    if (server) {
        gNetworkManager.startServer();
    } else {
        gNetworkManager.connectToServer("localhost", 1234);
    }

    double lastTickTime = 0.0;
    double tickRate = 1 / 60.0;
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTickTime;

    // Main loop
    while (server || !gRenderManager.shouldClose()) {
        currentTime = glfwGetTime();
        delta = currentTime - lastTickTime;
        if (delta < tickRate) continue;

        lastTickTime = currentTime;
        while (delta >= tickRate) {
            if (server) {
                gPhysicsManager.update();
                gNetworkManager.update();
                gEntityManager.update();
            } else {
                gRenderManager.frameStart();

                gPhysicsManager.update();
                gNetworkManager.update();
                gEntityManager.update();
                gRenderManager.update();
                gSoundManager.update();

                gRenderManager.frameEnd();
            }

            delta -= tickRate;
        }
    }

    // Shutdown subsystems
    gSoundManager.shutDown();
    gEntityManager.shutDown();
    gPhysicsManager.shutDown();
    gNetworkManager.shutDown();
    if (!server) {
        gInputManager.shutDown();
        gRenderManager.shutDown();
    }
    gAssetManager.shutDown();

#ifndef NDEBUG // Dump profile data
    profiler::dumpBlocksToFile("./profile_data.prof");
#endif

    return 0;
}