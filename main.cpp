#include <iostream>
#include <lua.hpp>

int main() {
    lua_State *state = luaL_newstate();
    luaL_openlibs(state);

    std::string filename = "test.lua";
    int err;
    err = luaL_loadfile(state, filename.c_str()) || lua_pcall(state, 0, 0, 0);

    if (err) {
        std::cout << lua_tostring(state, -1) << std::endl;
        lua_pop(state, 1);
    }

    lua_close(state);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}