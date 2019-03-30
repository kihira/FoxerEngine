
#ifndef FOXERENGINE_KEYHANDLER_H
#define FOXERENGINE_KEYHANDLER_H

#include <vector>
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

	/**
	 * Register a cursor handler that is called every time a cursor event is fired
	 * If the registered function returns true, then it stops further processing of inputs
	 */
    void registerCursorHandler(sol::function handler);

	/**
	 * INTERNAL
	 * Used for receiving callback from glfw
	 */
    void keyCallback(int key, int scancode, int action, int mods);

	/**
	 * INTERNAL
	 * Used for receiving callback from glfw
	 */
    void cursorCallback(double xPos, double yPos);
};


#endif //FOXERENGINE_KEYHANDLER_H
