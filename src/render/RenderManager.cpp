
#include <easy/profiler.h>
#include "RenderManager.h"
#include "../gl_helper.hpp"
#include "../Managers.h"

RenderManager::RenderManager() = default;

RenderManager::~RenderManager() = default;

void RenderManager::startUp() {
    logger = spdlog::stdout_color_mt("renderer");

    glfwSetErrorCallback(glfwErrorCallback);

    /*
     * InitializeGLFW
     */
    logger->info("GLFW {}", glfwGetVersionString());
    if (!glfwInit()) {
        logger->error("Failed to init GLFW");
        exit(EXIT_FAILURE);
    }

    // Required OpenGL 3.2 Core at least
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
#endif

    // Create GLFW window
    windowWrapper = std::make_unique<WindowWrapper>(glfwCreateWindow(720, 405, "301CR Engine", nullptr, nullptr));
    if (!windowWrapper->getWindow()) {
        logger->error("Failed to create GLFW window");
        exit(EXIT_FAILURE);
    }

    windowWrapper->registerFramebufferSizeCallback(RenderManager::glfwFramebufferSizeCallback);
    glfwMakeContextCurrent(windowWrapper->getWindow());
    glfwSwapInterval(1);

    // Init OpenGL and GLAD
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        logger->error("Failed to initialize OpenGL context");
        exit(EXIT_FAILURE);
    }
    logger->info("OpenGL {0:d}.{0:d}", GLVersion.major, GLVersion.minor);

    // Create camera
    camera = std::make_unique<Camera>(glm::vec3(0, 0, -3), glm::vec3(0, 0, 0), 45.f);
    camera->resize(720, 405);
    camera->setPosition(glm::vec3(5.f, 5.f, -5.f));
    camera->setTarget(glm::vec3(0.f));

    // Set OpenGL state stuff
    glClearColor(0.5, 0.5, 0, 1);
    glDisable(GL_CULL_FACE);
}

void RenderManager::shutDown() {
    glfwDestroyWindow(windowWrapper->getWindow());
    glfwTerminate();
}

void RenderManager::useShader(std::shared_ptr<Shader> shader) {
    if (shader->getProgram() == currentShader) return;

    glUseProgram(shader->getProgram());
    shader->setUniform("view", camera->getView());
    shader->setUniform("projection", camera->getProjection());
    GLERRCHECK();
}

void RenderManager::frameStart() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // NOTIDY
    GLERRCHECK();
}

void RenderManager::frameEnd() {
    windowWrapper->swapBuffers();
    glfwPollEvents();
}

void RenderManager::update() {
    for (auto renderComponent : renderComponents) {
        renderComponent->update();
    }
}

int RenderManager::shouldClose() {
    return windowWrapper->shouldClose();
}

const std::unique_ptr<WindowWrapper> &RenderManager::getWindowWrapper() const {
    return windowWrapper;
}

void RenderManager::addRenderComponent(RenderComponent *component) {
    renderComponents.push_back(component);
}

/*
 * Static functions
 */

void RenderManager::glfwErrorCallback(int error, const char *desc) {
    spdlog::get("renderer")->error("GLFW Error 0x{0:x}: {}", error, desc);
}

void RenderManager::glfwFramebufferSizeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    gRenderManager.camera->resize(width, height);
}
