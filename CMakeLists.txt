cmake_minimum_required(VERSION 3.04)
project(GameEngine301CR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(LIBRARIES_ROOT "${CMAKE_SOURCE_DIR}/libs")

# Enable building with profiler in debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DBUILD_WITH_EASY_PROFILER)
    add_definitions(-DSPDLOG_DEBUG_ON)
endif()

# Source files
set(SOURCES src/main.cpp src/entity/Entity.cpp src/entity/Entity.h src/AssetManager.cpp src/AssetManager.h src/render/Mesh.cpp src/render/Mesh.h src/render/Shader.cpp src/render/Shader.h src/gl_helper.hpp src/network/NetworkManager.cpp src/network/NetworkManager.h src/InputManager.cpp src/InputManager.h src/render/Camera.cpp src/render/Camera.h src/vectors.h src/level.cpp src/level.h src/assert.h src/render/RenderManager.cpp src/render/RenderManager.h src/entity/EntityManager.cpp src/entity/EntityManager.h src/render/WindowWrapper.cpp src/render/WindowWrapper.h src/entity/Player.cpp src/entity/Player.h src/Managers.h src/physics/PhysicsManager.cpp src/physics/PhysicsManager.h src/physics/PhysicsComponent.cpp src/physics/PhysicsComponent.h src/entity/Component.h src/network/NetworkComponent.cpp src/network/NetworkComponent.h src/SoundManager.cpp src/SoundManager.h src/Managers.cpp src/render/RenderComponent.cpp src/render/RenderComponent.h src/Settings.h)

add_executable(${PROJECT_NAME} ${SOURCES})

# Lua
if (WIN32)
	if (MSVC)
		target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/lua/lib/msvc/lua53.lib)
	else()
		target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/lua/lib/mingw/lua53.lib)
	endif()
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
add_subdirectory(${LIBRARIES_ROOT}/enet)
target_link_libraries(${PROJECT_NAME} enet)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/enet/include)

# Box2D
add_subdirectory(${LIBRARIES_ROOT}/box2d)
target_link_libraries(${PROJECT_NAME} Box2D)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/box2d/)

# GLM
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/glm)

# sol2
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/sol)

# Imgui
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/imgui)

# Easy Profiler
set(EASY_PROFILER_NO_GUI ON)
set(EASY_PROFILER_NO_SAMPLES ON)
add_subdirectory(${LIBRARIES_ROOT}/easy_profiler)
target_link_libraries(${PROJECT_NAME} easy_profiler)

# Soloud
set(SOLOUD_BACKEND_SDL2 OFF)
set(SOLOUD_STATIC ON)
if (WIN32)
	if (MSVC)
		# https://cmake.org/pipermail/cmake/2009-September/032204.html
		#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /arch:SSE2")
		#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2")
	else()
	    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} "-msse3")
		set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-msse3")
	endif()
    set(SOLOUD_BACKEND_WASAPI ON)
elseif(APPLE)
    set(SOLOUD_BACKEND_COREAUDIO ON)
endif()
add_subdirectory(${LIBRARIES_ROOT}/soloud/contrib)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/soloud/include)
target_link_libraries(${PROJECT_NAME} soloud)

# stb headers
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/stb)

# spdlog
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_BENCH OFF CACHE BOOL "" FORCE)
set(SPDLOG_BUILD_TESTING OFF CACHE BOOL "" FORCE)

add_subdirectory(${LIBRARIES_ROOT}/spdlog)
target_link_libraries(${PROJECT_NAME} spdlog)

# assimp
#set(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
#set(ASSIMP_BUILD_TESTS OFF)
#set(VIEWER_BUILD OFF)
## disable exporting from assimp and only enabled importing obj files for now
#set(ASSIMP_NO_EXPORT ON)
#set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF)
#set(ASSIMP_BUILD_OBJ_IMPORTER ON)
#add_subdirectory(${LIBRARIES_ROOT}/assimp)
#target_link_libraries(${PROJECT_NAME} assimp)

# Symlink assets to an output resources folder
set(SOURCE_RESOURCES ${CMAKE_SOURCE_DIR}/assets)
set(DESTINATION_RESOURCES $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets)

if (WIN32)
	add_custom_command(
			TARGET ${PROJECT_NAME}
			POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_RESOURCES} ${DESTINATION_RESOURCES}
			DEPENDS ${DESTINATION_RESOURCES})
endif()

add_definitions(-DASSERTIONS_ENABLED=1)