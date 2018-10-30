
#ifndef GAMEENGINE301CR_WINDOWMANAGER_H
#define GAMEENGINE301CR_WINDOWMANAGER_H

#include <GLFW/glfw3.h>

typedef void (* FramebufferSizeFn)(int width, int height);


/**
 * A wrapper class that goes around GLFWwindow.
 * This provides a slightly nicer interface and more C++ compatability
 */
class WindowWrapper {
private:
    GLFWwindow *window;
    FramebufferSizeFn framebufferSizeCallback;
public:
    WindowWrapper(GLFWwindow *window);

    void registerFramebufferSizeCallback(FramebufferSizeFn callback);

    GLFWwindow *getWindow() const;

    void swapBuffers();

    int shouldClose();
};


#endif //GAMEENGINE301CR_WINDOWMANAGER_H
