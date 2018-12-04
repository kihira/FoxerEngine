#include <easy/profiler.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Managers.h"

int main(int argc, char **argv) {
#ifndef NDEBUG // Enable profiler in debug mode
    EASY_PROFILER_ENABLE;
#endif
    bool server = argc == 2; // Whether we are a server or not

    // Initialise logger
    auto logger = spdlog::stdout_color_mt("main");
    logger->info("Starting as {}", server ? "server" : "client");

    gAssetManager.startUp();
    if (!server) {
        gRenderManager.startUp();
        gInputManager.startUp();
    }
    gNetworkManager.startUp();
    gPhysicsManager.startUp();
    gEntityManager.startUp();
    gSoundManager.startUp();

    // Register entity prototypes
    gAssetManager.loadEntityPrototype("testEntity", "testEntity");

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