cmake_minimum_required(VERSION 3.04)
project(GameEngine301CR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(LIBRARIES_ROOT "${CMAKE_SOURCE_DIR}/libs")

# Enable building with profiler in debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DBUILD_WITH_EASY_PROFILER)
endif()

# Source files
set(SOURCES src/main.cpp src/entity/Entity.cpp src/entity/Entity.h src/AssetManager.cpp src/AssetManager.h src/render/Mesh.cpp src/render/Mesh.h src/render/Shader.cpp src/render/Shader.h src/gl_helper.hpp src/network/NetworkManager.cpp src/network/NetworkManager.h src/KeyHandler.cpp src/KeyHandler.h src/render/Camera.cpp src/render/Camera.h src/vectors.h src/level.cpp src/level.h src/assert.h src/render/RenderManager.cpp src/render/RenderManager.h src/entity/EntityManager.cpp src/entity/EntityManager.h src/render/WindowWrapper.cpp src/render/WindowWrapper.h src/entity/Player.cpp src/entity/Player.h src/Managers.h src/physics/PhysicsManager.cpp src/physics/PhysicsManager.h src/physics/PhysicsComponent.cpp src/physics/PhysicsComponent.h src/entity/Component.h src/network/NetworkComponent.cpp src/network/NetworkComponent.h src/SoundManager.cpp src/SoundManager.h src/Managers.cpp)

add_executable(${PROJECT_NAME} ${SOURCES})

# Lua
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/lua/lib/lua53.lib)
else()
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/lua/lib/liblua53.a)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/lua/include)

# GLAD
add_library(glad ${LIBRARIES_ROOT}/glad/src/glad.c)
target_include_directories(glad PRIVATE ${LIBRARIES_ROOT}/glad/include)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/glad/include)
target_link_libraries(${PROJECT_NAME} glad)

# GLFW
# Disable GLFW docs, tests and examples
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${LIBRARIES_ROOT}/glfw)
target_link_libraries(${PROJECT_NAME} glfw)

# Enet
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/enet/enet.lib)
    target_link_libraries(${PROJECT_NAME} ws2_32 winmm) # Winsock32 libraries required by enet
else()
    add_subdirectory(${LIBRARIES_ROOT}/enet)
    target_link_libraries(${PROJECT_NAME} enet)
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/enet/include)

# Box2D
add_subdirectory(${LIBRARIES_ROOT}/box2d)
target_link_libraries(${PROJECT_NAME} Box2D)
target_include_directories(${PROJECT_NAME} PRIVATE ${Box2D_SOURCE_DIR})

# FMOD
#if (APPLE)
#    if(CMAKE_BUILD_TYPE MATCHES DEBUG)
#        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/lowlevel/lib/libfmodL.dylib)
#    else()
#        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/lowlevel/lib/libfmod.dylib)
#    endif()
#else()
##    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/lowlevel/lib/fmod_vc.lib)
#    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/lowlevel/lib/libfmod.a)
#endif()
#target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/fmod/lowlevel/inc)

# GLM
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/glm)

# sol2
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/sol)

# Imgui
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/imgui)

# Easy Profiler
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/easyprofiler/include)
target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/easyprofiler/macos/libeasy_profiler.dylib)

# Soloud
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/soloud/include)
if (APPLE)
    # This was a pain to fix. Needed to manually compile and link this way. Can't just find CoreAudio
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/soloud/lib/libsoloud_static_x86_d.a)
    target_link_libraries(${PROJECT_NAME} "-framework CoreAudio")
    target_link_libraries(${PROJECT_NAME} "-framework AudioToolbox")
else()
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/soloud/lib/soloud_static_x86.lib)
endif()

# Symlink assets to an output resources folder
set(SOURCE_RESOURCES ${CMAKE_SOURCE_DIR}/assets)
set(DESTINATION_RESOURCES $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_RESOURCES} ${DESTINATION_RESOURCES}
        DEPENDS ${DESTINATION_RESOURCES})

add_definitions(-DASSERTIONS_ENABLED=1)