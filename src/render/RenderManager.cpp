
#include "RenderManager.h"
#include "../gl_helper.hpp"

// todo should this just be called from the Shader itself?
void RenderManager::useShader(std::shared_ptr<Shader> shader) {
    if (shader->getProgram() == currentShader) return;

    glUseProgram(shader->getProgram());
}

void RenderManager::startUp() {
    // todo glfwSetErrorCallback(glfwErrorCallback);

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
    window = glfwCreateWindow(720, 405, "301CR Engine", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    // todo glfwSetFramebufferSizeCallback(window, [&](GLFWwindow *window, int width, int height) { this->glfwFramebufferSizeCallback(window, width, height); });
    glfwSwapInterval(1);

    // Init OpenGL and GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::cout << "OpenGL " << GLVersion.major << "." << GLVersion.minor << std::endl;

    // Create camera
    camera = std::make_unique<Camera>(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), 45.f);
    camera->resize(720, 405);

    // Set OpenGL state stuff
    glClearColor(0.5, 0.5, 0, 1);
    glDisable(GL_CULL_FACE);
}

void RenderManager::shutDown() {

}

void RenderManager::frameStart() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOTIDY
    GLERRCHECK();
}

void RenderManager::frameEnd() {
    glfwSwapBuffers(window);
    glfwPollEvents();
}

void RenderManager::glfwErrorCallback(int error, const char *desc) {
    std::cerr << "GLFW Error 0x" << std::hex << error << ": " << desc << std::endl;
}

void RenderManager::glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
    camera->resize(width, height);
}

GLFWwindow *RenderManager::getWindow() const {
    return window;
}

void RenderManager::update() {

}

inline bool RenderManager::shouldClose() {
    return glfwWindowShouldClose(window);
}
