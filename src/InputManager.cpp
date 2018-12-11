
#include "InputManager.h"
#include "Managers.h"
#include "render/RenderManager.h"

void InputManager::startUp() {
    gRenderManager.getWindowWrapper()->registerKeyCallback([](int key, int scancode, int action, int mods) {
        gInputManager.keyCallback(key, scancode, action, mods);
    });
    gRenderManager.getWindowWrapper()->registerCursorCallback([](double xPos, double yPos) {
        gInputManager.cursorCallback(xPos, yPos);
    });
}

void InputManager::shutDown() {

}

void InputManager::registerKeyHandler(sol::function handler) {
    keyHandlers.emplace_back(handler);
}

void InputManager::registerCursorHandler(sol::function handler) {
    cursorHandlers.emplace_back(handler);
}

void InputManager::keyCallback(int key, int scancode, int action, int mods) {
    for (auto& keyHandler: keyHandlers) {
        if (keyHandler(key, scancode, action, mods)) break;
    }
}

void InputManager::cursorCallback(double xPos, double yPos) {
    for (auto& handler: cursorHandlers) {
        if (handler(xPos, yPos)) break;
    }
}
