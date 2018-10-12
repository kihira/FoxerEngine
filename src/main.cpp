#define SOL_CHECK_ARGUMENTS 1

#include <iostream>
#include <sol.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <enet/enet.h>
#include "SoundManager.h"

void glfwErrorCallback(int error, const char *desc) {
    std::cerr << "GLFW Error " << error << ": " << desc << std::endl;
}

void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    // todo handle resize
}

sol::protected_function_result luaErrorCallback(lua_State*, sol::protected_function_result pfr) {
    return pfr;
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
        glfwTerminate();
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
     * Initialise networking
     */
    std::cout << "ENet " << ENET_VERSION_MAJOR << "." << ENET_VERSION_MINOR << "." << ENET_VERSION_PATCH << std::endl;
    if (enet_initialize() != 0) {
        std::cerr << "Failed to intialize ENet" << std::endl;
        exit(EXIT_FAILURE);
    }
    atexit(enet_deinitialize);

    /*
     * Initialise sound system
     */
    auto *soundManager = new SoundManager();

    // Test lua
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::io);

    lua.script_file("resources/scripts/test.lua", luaErrorCallback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOTIDY

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}