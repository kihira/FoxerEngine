# Foxer Engine
## Demo
An example demo game and binary can be found in the demo script file.

The demo is a multiplayer game requiring two clients to connect before it will begin.
The goal is to hit all the checkpoints first

## Libraries
- GLFW 3.2.1 [zlib](http://www.glfw.org/license.html)
- GLM 0.9.9.2 [MIT](https://github.com/g-truc/glm/blob/master/manual.md#section0)
- Lua 5.3.5 [MIT](https://www.lua.org/license.html)
- Sol2 2.20.3 [MIT](https://sol2.readthedocs.io/en/latest/licenses.html)
- ENet 1.3.13 [MIT](http://enet.bespin.org/License.html)
- Box2D 2.3.1 [zlib](https://github.com/erincatto/Box2D/blob/master/LICENSE)
- ImGui 1.65 [MIT](https://github.com/ocornut/imgui/blob/master/LICENSE.txt)
- easy_profiler 2.0.1 [MIT](https://github.com/yse/easy_profiler/blob/develop/LICENSE.MIT)
- spdlog 1.2.1 [MIT](https://github.com/gabime/spdlog/blob/v1.x/LICENSE)
- stb_image.h 2.19 [Public Domain](https://github.com/nothings/stb/blob/master/docs/why_public_domain.md)
- soloud 20181119 [zlib](https://github.com/jarikomppa/soloud/blob/master/LICENSE)
- tinyobjloader 1.0.6 [MIT](https://github.com/syoyo/tinyobjloader/blob/master/LICENSE)

## Build
All the libraries are contained within the repo and building the project
should be as simple as just building the CMake project.

You are able to open CMakeLists.txt inside of Visual Studio 2017 or you
can generate the required file for your IDE using the `cmake -G` command.