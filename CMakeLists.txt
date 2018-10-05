cmake_minimum_required(VERSION 3.12)
project(301CR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

# lua
if (APPLE)
    set(LUA_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lua/lib/liblua53.a)
else()
    set(LUA_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/lua/lib/lua53.dll)
endif()
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lua/include)

# GLFW
# Disable GLFW docs, tests and examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/glfw)

# GLM
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/glm)

add_executable(301CR main.cpp)

# Link libaries
target_link_libraries(301CR ${LUA_LIBRARIES})
target_link_libraries(301CR glfw)