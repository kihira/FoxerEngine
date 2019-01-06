#include <easy/profiler.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Managers.h"
#include "AssetManager.h"
#include "physics/PhysicsManager.h"
#include "network/NetworkManager.h"
#include "render/RenderManager.h"
#include "entity/EntityManager.h"
#include "sound/SoundManager.h"
#include "InputManager.h"
#include "event/EventManager.h"
#include "level/LevelManager.h"

void runMainLoop() {
    double lastTickTime = 0.0;
    double tickRate = gAssetManager.loadSettings()->tickRate;
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTickTime;

    // Main loop
    while (!gRenderManager.shouldClose()) {
        currentTime = glfwGetTime();
        delta = currentTime - lastTickTime;
        if (delta < tickRate) continue;

        lastTickTime = currentTime;
        while (delta >= tickRate) {
            gRenderManager.frameStart();

            gPhysicsManager.update(delta);
            gNetworkManager.update(delta);
            gEntityManager.update(delta);
			gLevelManager.update();
            gRenderManager.update(delta);
            gSoundManager.update(delta);

            gRenderManager.frameEnd();

            delta -= tickRate;
        }
    }
}

void runMainLoopServer() {
    double lastTickTime = 0.0;
    double tickRate = gAssetManager.loadSettings()->tickRate;
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTickTime;

    // Main loop
    while (true) {
        currentTime = glfwGetTime();
        delta = currentTime - lastTickTime;
        if (delta < tickRate) continue;

        lastTickTime = currentTime;
        while (delta >= tickRate) {
            gPhysicsManager.update(delta);
            gNetworkManager.update(delta);
            gEntityManager.update(delta);
			gLevelManager.update();

            delta -= tickRate;
        }
    }
}

int main(int argc, char **argv) {
#ifndef NDEBUG // Enable profiler in debug mode
    EASY_PROFILER_ENABLE;
    spdlog::set_level(spdlog::level::debug);
#endif
    bool server = argc == 2; // Whether we are a server or not

    // Initialise logger
    auto logger = spdlog::stdout_color_mt("main");
    logger->info("Starting as {}", server ? "server" : "client");

    gEventManager.startUp();
    gAssetManager.startUp();
    //if (!server) {
        gRenderManager.startUp();
        gInputManager.startUp();
    // }
    gNetworkManager.startUp();
    gPhysicsManager.startUp();
    gEntityManager.startUp();
    gSoundManager.startUp();
    gLevelManager.startUp();

    // Start as server or connect to one
    if (server) {
        // Load initial level
        gLevelManager.loadLevel(gAssetManager.loadSettings()->initialLevel);

        gNetworkManager.startServer();
    } else {
        gNetworkManager.connectToServer("localhost", 1234);
    }

    logger->info("Starting main loop...");
    if (server) {
        runMainLoopServer();
    } else {
        runMainLoop();
    }

    // Shutdown subsystems
    gLevelManager.shutDown();
    gSoundManager.shutDown();
    gEntityManager.shutDown();
    gPhysicsManager.shutDown();
    gNetworkManager.shutDown();
    if (!server) {
        gInputManager.shutDown();
        gRenderManager.shutDown();
    }
    gAssetManager.shutDown();
    gEventManager.shutDown();

#ifndef NDEBUG // Dump profile data
    profiler::dumpBlocksToFile("./profile_data.prof");
#endif

    // Shutdown loggers
    spdlog::drop_all();

    return 0;
}