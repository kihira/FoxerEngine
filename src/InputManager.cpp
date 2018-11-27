
#include "Managers.h"
#include "InputManager.h"

void InputManager::startUp() {
    gRenderManager.getWindowWrapper()->registerKeyCallback([](int key, int scancode, int action, int mods) {
        gInputManager.keyCallback(key, scancode, action, mods);
    });
}

void InputManager::shutDown() {

}

void InputManager::registerKeyHandler(KeyHandlerFn keyHandler) {
    keyHandlers.push_back(keyHandler);
}

void InputManager::registerKeyHandlerLua(sol::protected_function callback) {
    luaKeyHandlers.emplace_back(callback);
}

void InputManager::keyCallback(int key, int scancode, int action, int mods) {
    for (auto& keyHandler: keyHandlers) {
        if (keyHandler(key, scancode, action, mods)) break;
    }

    for (auto& keyHandler: luaKeyHandlers) {
        if (keyHandler(key, scancode, action, mods)) break;
    }
}
