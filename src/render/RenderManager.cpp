
#include <easy/profiler.h>
#include "RenderManager.h"
#include "../Managers.h"
#include "../AssetManager.h"

RenderManager::RenderManager() = default;

RenderManager::~RenderManager() = default;

void RenderManager::startUp() {
    logger = spdlog::get("main")->clone("renderer");

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

    auto settings = gAssetManager.loadSettings();

    // Create GLFW window
    windowWrapper = std::make_unique<WindowWrapper>(glfwCreateWindow(settings->windowWidth, settings->windowHeight, settings->windowTitle.c_str(), nullptr, nullptr));
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
    logger->info("OpenGL {:d}.{:d}", GLVersion.major, GLVersion.minor);

    // Create camera
    camera = std::make_unique<Camera>(glm::vec3(0, 3, -3), glm::vec3(0, 0, 0), settings->cameraFov);
    camera->resize(settings->windowWidth, settings->windowHeight);

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
        if (!renderComponent->isActive()) continue;
        renderComponent->update();
    }
}

int RenderManager::shouldClose() {
    return windowWrapper->shouldClose();
}

WindowWrapper *RenderManager::getWindowWrapper() const {
    return windowWrapper.get();
}

void RenderManager::addRenderComponent(RenderComponent *component) {
    renderComponents.push_back(component);
}

/*
 * Static functions
 */

void RenderManager::glfwErrorCallback(int error, const char *desc) {
    spdlog::get("renderer")->error("GLFW Error 0x{:x}: {}", error, desc);
}

void RenderManager::glfwFramebufferSizeCallback(int width, int height) {
    glViewport(0, 0, width, height);
    gRenderManager.camera->resize(width, height);
}

void RenderManager::GLERRCHECK_fn(const char *file, int line) {
    GLenum err = glGetError();
    if (err == GL_NO_ERROR) return;

    std::string error;

    switch (err) {
        case GL_INVALID_ENUM:
            error = "INVALID ENUM";
            break;
        case GL_INVALID_VALUE:
            error = "INVALID VALUE";
            break;
        case GL_INVALID_OPERATION:
            error = "INVALID OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            error = "OUT OF MEMORY";
            break;
        default:
            error = "UNKNOWN ERROR";
            break;
    }

    spdlog::get("renderer")->error("({}:{}) GL ERROR 0x{:x}: {}", file, line, err, error);
}

const Camera *RenderManager::getCamera() const {
    return camera.get();
}
