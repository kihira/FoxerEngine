
#include "WindowWrapper.h"
#include "../assert.h"

WindowWrapper::WindowWrapper(GLFWwindow *window) : window(window) {
    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        auto wrapper = static_cast<WindowWrapper *>(glfwGetWindowUserPointer(window));
        wrapper->framebufferSizeCallback(width, height);
    });
}

void WindowWrapper::registerFramebufferSizeCallback(FramebufferSizeFn callback) {
    framebufferSizeCallback = callback;
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