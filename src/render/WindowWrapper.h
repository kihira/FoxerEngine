
#ifndef GAMEENGINE301CR_WINDOWMANAGER_H
#define GAMEENGINE301CR_WINDOWMANAGER_H

#include <GLFW/glfw3.h>

// Use custom definitions to avoid passing around GLFW window
typedef void (* FramebufferSizeFn)(int width, int height);
typedef void (* KeyFn)(int key, int scancode, int action, int mods);
typedef void (* CursorFn)(double xPos, double yPos);


/**
 * A wrapper class that goes around GLFWwindow.
 * This provides a slightly nicer interface and easier handling of GLFW callbacks
 */
class WindowWrapper {
private:
    GLFWwindow *window;
    FramebufferSizeFn framebufferSizeCallback;
    KeyFn keyCallback;
    CursorFn cursorCallback;
public:
    explicit WindowWrapper(GLFWwindow *window);

    /**
     * Registers a function that will be called when the framebuffer size changes
     * @param callback
     */
    void registerFramebufferSizeCallback(FramebufferSizeFn callback);

    /**
     * Registers a function that will be called when a key is pressed
     * @param callback
     */
    void registerKeyCallback(KeyFn callback);

    void registerCursorCallback(CursorFn callback);

    GLFWwindow *getWindow() const;

    void swapBuffers();

    int shouldClose();
};


#endif //GAMEENGINE301CR_WINDOWMANAGER_H
