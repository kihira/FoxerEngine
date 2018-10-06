#define SOL_CHECK_ARGUMENTS 1

#include <iostream>
#include <sol.hpp>
#include <Box2D/Box2D.h>
#include <chipmunk/chipmunk.h>

int main() {
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::io);

    auto handler = &sol::script_default_on_error;

    lua.script_file("resources/scripts/test.lua", handler);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}