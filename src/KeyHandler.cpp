
#include "KeyHandler.h"

void KeyHandler::registerKeyHandler(KeyHandlerFn keyHandler) {
    keyHandlers.push_back(keyHandler);
}

void KeyHandler::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    // Retrieve the pointer to this class
    KeyHandler *handler = static_cast<KeyHandler *>(glfwGetWindowUserPointer(window));

    for (auto& keyHandler: handler->keyHandlers) {
        if (keyHandler(key, scancode, action, mods)) break;
    }
}