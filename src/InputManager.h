
#ifndef GAMEENGINE301CR_KEYHANDLER_H
#define GAMEENGINE301CR_KEYHANDLER_H

#include <vector>
#include <GLFW/glfw3.h>
#include <sol.hpp>

typedef bool (* KeyHandlerFn)(int key, int scancode, int action, int mods);

class InputManager {
private:
    std::vector<KeyHandlerFn> keyHandlers;
    std::vector<sol::function> luaKeyHandlers;
public:
    void startUp();

    void shutDown();
    /**
     * Register a key handler that is called every time a key event is fired.
     * If the registered function returns true, then it stops further processing of inputs
     */
    void registerKeyHandler(KeyHandlerFn keyHandler);

    /**
     * Function for binding lua callbacks
     */
    void registerKeyHandlerLua(sol::protected_function callback);

    void keyCallback(int key, int scancode, int action, int mods);
};


#endif //GAMEENGINE301CR_KEYHANDLER_H
