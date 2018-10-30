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
    // todo
//    auto keyHandler = std::make_unique<KeyHandler>();
//    glfwSetWindowUserPointer(renderManager->getWindow(), keyHandler.get());
//    glfwSetKeyCallback(renderManager->getWindow(), KeyHandler::keyCallback);

    sol::table engineTable = assetManager->getLua().create_named_table("engine"); // Namespace for interacting with the engine

    // Load functions for lua
    engineTable.set_function("registerEntityPrototype", [&assetManager](std::string fileName, std::string tableName) -> std::shared_ptr<Entity> { return assetManager->loadEntityPrototype(fileName, tableName); });
    engineTable.set_function("spawnEntity", [&entityManager](std::string id) -> std::shared_ptr<Entity> { return entityManager->spawn(id); });
    // TODO could also do this but need to remove unique ptr
    // engineTable.set_function("spawnEntity", &EntityManager::spawn, entityManager);

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
            "spawn", [&entityManager](std::string name){return entityManager->spawn(name);}, // Provides a method for retrieving a copy of a prototype
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
    entityManager->registerPrototype("player", assetManager->loadEntityPrototype("entity", "testEntity"));

    auto level = assetManager->loadLevel("level1");

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