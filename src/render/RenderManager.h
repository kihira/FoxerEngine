
#ifndef GAMEENGINE301CR_RENDERMANAGER_H
#define GAMEENGINE301CR_RENDERMANAGER_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"

class RenderManager {
private:
    GLFWwindow *window;
    std::unique_ptr<Camera> camera;
    GLuint currentShader;

    void glfwErrorCallback(int error, const char *desc);

    void glfwFramebufferSizeCallback(GLFWwindow *window, int width, int height);
public:
    void startUp();

    void shutDown();

    void frameStart();

    void frameEnd();

    void update();

    /**
     * Checks whether the GLFW window is attempting to close
     * @return
     */
    bool shouldClose();

    GLFWwindow *getWindow() const;

    void useShader(std::shared_ptr<Shader> shader);
};


#endif //GAMEENGINE301CR_RENDERMANAGER_H
