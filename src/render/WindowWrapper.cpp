
#include "WindowWrapper.h"
#include "../assert.h"

WindowWrapper::WindowWrapper(GLFWwindow *window) : window(window) {
    ASSERT(window != nullptr);

    glfwSetWindowUserPointer(window, this);
}

void WindowWrapper::registerFramebufferSizeCallback(FramebufferSizeFn callback) {
    ASSERT(callback != nullptr);
    framebufferSizeCallback = callback;

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        auto wrapper = static_cast<WindowWrapper *>(glfwGetWindowUserPointer(window));
        wrapper->framebufferSizeCallback(width, height);
    });
}

void WindowWrapper::registerKeyCallback(KeyFn callback) {
    ASSERT(callback != nullptr);
    keyCallback = callback;

    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto wrapper = static_cast<WindowWrapper *>(glfwGetWindowUserPointer(window));
        wrapper->keyCallback(key, scancode, action, mods);
    });
}

void WindowWrapper::registerCursorCallback(CursorFn callback) {
    ASSERT(callback != nullptr);
    cursorCallback = callback;

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xPos, double yPos) {
        auto wrapper = static_cast<WindowWrapper *>(glfwGetWindowUserPointer(window));
        wrapper->cursorCallback(xPos, yPos);
    });
}

GLFWwindow *WindowWrapper::getWindow() const {
    return window;
}

void WindowWrapper::swapBuffers() {
    glfwSwapBuffers(window);
}

int WindowWrapper::shouldClose() {
    return glfwWindowShouldClose(window);
}