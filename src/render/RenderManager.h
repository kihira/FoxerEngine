
#ifndef GAMEENGINE301CR_RENDERMANAGER_H
#define GAMEENGINE301CR_RENDERMANAGER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "Shader.h"
#include "Camera.h"
#include "WindowWrapper.h"
#include "RenderComponent.h"

#define GLERRCHECK() RenderManager::GLERRCHECK_fn(__FILE__, __LINE__)

class RenderManager {
private:
    std::unique_ptr<WindowWrapper> windowWrapper;
    std::unique_ptr<Camera> camera;
    GLuint currentShader = 0;
    std::shared_ptr<spdlog::logger> logger;
    std::vector<RenderComponent *> renderComponents;

    static void glfwErrorCallback(int error, const char *desc);

    static void glfwFramebufferSizeCallback(int width, int height);
public:
    RenderManager();

    ~RenderManager();

    /**
     * Initialises the RenderManager
     */
    void startUp();

    /**
     * De-initialises the RenderManager
     */
    void shutDown();

    void frameStart();

    void frameEnd();

    void update();

    /**
     * Checks whether the GLFW window is attempting to close
     * @return
     */
    int shouldClose();

    /**
     * Begins using the shader and sets up the view and projection matrices
     * @param shader
     */
    void useShader(std::shared_ptr<Shader> shader);

    void addRenderComponent(RenderComponent *component);

    WindowWrapper *getWindowWrapper() const;

    const Camera *getCamera() const;

    static void GLERRCHECK_fn(const char *file, int line);
};


#endif //GAMEENGINE301CR_RENDERMANAGER_H
