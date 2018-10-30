#include <memory>
#include <iostream>
#include <sol.hpp>
#include "render/RenderManager.h"
#include "KeyHandler.h"
#include "AssetManager.h"
#include "EntityManager.h"

int main() {
    auto renderManager = std::make_unique<RenderManager>();
    auto assetManager = std::make_unique<AssetManager>();
    auto entityManager = std::make_unique<EntityManager>();

    assetManager->startUp();
    renderManager->startUp();
    entityManager->startUp();

    /*
     * Setup key handler
     */
    auto keyHandler = std::make_unique<KeyHandler>();
    glfwSetWindowUserPointer(renderManager->getWindow(), keyHandler.get());
    glfwSetKeyCallback(renderManager->getWindow(), KeyHandler::keyCallback);

    sol::table engineTable = assetManager->getLua().create_named_table("engine"); // Namespace for interacting with the engine
    sol::table entitiesTable = assetManager->getLua().create_named_table("entities");

    // Load functions for lua
    engineTable["registerEntityPrototype"] = &AssetManager::getEntityPrototype;

    // Register entity type
    entitiesTable.new_usertype<Entity>(
            "entity",
            // sol::constructors<Entity(const char *)>(),
            "noconstructor", sol::no_constructor, // No constructor as we use factory
            "spawn", [&entityManager](std::string name){return entityManager->spawn(name);}, // Provides a method for retrieving a copy of a prototype
            // Register properties
            "position", sol::property(&Entity::getPosition, &Entity::setPosition),
            "rotation", sol::property(&Entity::getRotation, &Entity::setRotation)
            );

    engineTable.set_function("registerKeyHandler", [&keyHandler](sol::function callback) -> void {
        return keyHandler->registerKeyHandlerLua(callback);
    });

    // Main loop
    while (!renderManager->shouldClose()) {
        renderManager->frameStart();

        entityManager->update();
        renderManager->update();

        renderManager->frameEnd();
    }

    // Shutdown subsystems
    entityManager->shutDown();
    renderManager->shutDown();
    assetManager->shutDown();

    return 0;
}