
#ifndef GAMEENGINE301CR_RENDERMANAGER_H
#define GAMEENGINE301CR_RENDERMANAGER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include "WindowWrapper.h"

class RenderManager {
private:
    std::unique_ptr<WindowWrapper> windowWrapper;
    std::unique_ptr<Camera> camera;
    GLuint currentShader = 0;

    static std::shared_ptr<RenderManager> instance_;

    static void glfwErrorCallback(int error, const char *desc);

    static void glfwFramebufferSizeCallback(int width, int height);
public:
    static std::shared_ptr<RenderManager> instance();

    void startUp();

    void shutDown();

    void frameStart();

    void frameEnd();

    void update();

    /**
     * Checks whether the GLFW window is attempting to close
     * @return
     */
    int shouldClose();

    void useShader(std::shared_ptr<Shader> shader);

    const std::unique_ptr<WindowWrapper> &getWindowWrapper() const;
};


#endif //GAMEENGINE301CR_RENDERMANAGER_H
