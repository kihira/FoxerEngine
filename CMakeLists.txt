cmake_minimum_required(VERSION 3.12)
project(301CR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")

set(LIBRARIES_ROOT "${CMAKE_SOURCE_DIR}/libs")
set(LIBRARIES "")

# Lua
if (APPLE)
    list(APPEND LIBRARIES ${LIBRARIES_ROOT}/lua/lib/liblua53.a)
else()
    list(APPEND LIBRARIES ${LIBRARIES_ROOT}/lua/lib/lua53.dll)
endif()
include_directories(${LIBRARIES_ROOT}/lua/include)

# GLFW
# Disable GLFW docs, tests and examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${LIBRARIES_ROOT}/glfw)
list(APPEND LIBRARIES glfw)

# GLM
include_directories(${LIBRARIES_ROOT}/glm)

# sol2
include_directories(${LIBRARIES_ROOT}/sol)

# Enet
if (WIN32)
    list(APPEND LIBRARIES ${LIBRARIES_ROOT}/enet/enet.lib)
else()
    add_subdirectory(${LIBRARIES_ROOT}/enet)
endif()
include_directories(${LIBRARIES_ROOT}/enet/include)

# Add files to executable
add_executable(301CR src/main.cpp)

# Link libaries
target_link_libraries(301CR ${LUA_LIBRARIES})
target_link_libraries(301CR enet)

# Symlink assets to an output resources folder
set(SOURCE_RESOURCES ${CMAKE_SOURCE_DIR}/assets)
set(DESTINATION_RESOURCES $<TARGET_FILE_DIR:301CR>/resources)

add_custom_command(
        TARGET 301CR
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E create_symlink ${SOURCE_RESOURCES} ${DESTINATION_RESOURCES}
        DEPENDS ${DESTINATION_RESOURCES})

# Set resources folder
add_compile_definitions(RESOURCE_LOC=./resources/)