#include <memory>

#include <memory>

#define SOL_CHECK_ARGUMENTS 1

#include <iostream>
#include <sol.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <enet/enet.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_helper.hpp"
#include "KeyHandler.h"
#include "AssetManager.h"
#include "render/Camera.h"
#include "Entity.h"

// temp
std::unique_ptr<Camera> camera;
std::vector<Entity *> entities;

void glfwErrorCallback(int error, const char *desc) {
    std::cerr << "GLFW Error 0x" << std::hex << error << ": " << desc << std::endl;
}

void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera->resize(width, height);
}

sol::protected_function_result luaErrorCallback(lua_State *, sol::protected_function_result pfr) {
    return pfr;
}

void addEntity(Entity *entity) {
    entities.push_back(entity);
}

int main() {
    glfwSetErrorCallback(glfwErrorCallback);

    /*
     * InitializeGLFW
     */
    std::cout << "GLFW " << glfwGetVersionString() << std::endl;
    if (!glfwInit()) {
        std::cerr << "Failed to init GLFW" << std::endl;
        exit(EXIT_FAILURE);
    }

    atexit(glfwTerminate);

    // Required OpenGL 3.2 Core at least
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

    // Create GLFW window
    GLFWwindow *window = glfwCreateWindow(720, 405, "301CR Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, glfwFramebufferSizeCallback);
    glfwSwapInterval(1);

    // Init OpenGL and GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    /*
     * Setup key handler
     */
    auto keyHandler = std::make_unique<KeyHandler>();
    glfwSetWindowUserPointer(window, keyHandler.get());
    glfwSetKeyCallback(window, KeyHandler::keyCallback);

    /*
     * Create camera
     */
    camera = std::make_unique<Camera>(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), 45.f);
    camera->resize(720, 405);

    auto assetManager = std::make_unique<AssetManager>();

    /*
     * Load global lua space
     */
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::io);
    sol::table engine = lua.create_named_table("engine"); // Namespace for interacting with the engine

    // Load functions for lua
    engine["addEntity"] = addEntity;
    engine.new_usertype<Entity>(
            "entity",
            sol::constructors<Entity(const char *)>(),
            // Register methods
            "loadScript", &Entity::loadScript);

    // Test entity
    auto *entity = new Entity("Test");
    entity->loadScript(lua, "assets/scripts/entity.lua");
    entities.push_back(entity);

    glClearColor(0.5, 0.5, 0, 1);
    glDisable(GL_CULL_FACE);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOTIDY
        GLERRCHECK();

        for (auto &entity: entities) {
            entity->update();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}