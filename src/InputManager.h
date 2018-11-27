
#ifndef GAMEENGINE301CR_KEYHANDLER_H
#define GAMEENGINE301CR_KEYHANDLER_H

#include <vector>
#include <GLFW/glfw3.h>
#include <sol.hpp>

class InputManager {
private:
    std::vector<sol::function> keyHandlers;
    std::vector<sol::function> cursorHandlers;
public:
    void startUp();

    void shutDown();

    /**
     * Register a key handler that is called every time a key event is fired.
     * If the registered function returns true, then it stops further processing of inputs
     */
    void registerKeyHandler(sol::function handler);

    void registerCursorHandler(sol::function handler);

    void keyCallback(int key, int scancode, int action, int mods);

    void cursorCallback(double xPos, double yPos);
};


#endif //GAMEENGINE301CR_KEYHANDLER_H
