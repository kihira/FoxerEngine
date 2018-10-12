cmake_minimum_required(VERSION 3.12)
project(GameEngine301CR)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(LIBRARIES_ROOT "${CMAKE_SOURCE_DIR}/libs")

# Source files
set(SOURCES src/main.cpp src/Entity.cpp src/Entity.h src/AssetManager.cpp src/AssetManager.h src/Mesh.cpp src/Mesh.h src/Shader.cpp src/Shader.h src/SoundManager.cpp src/SoundManager.h)

add_executable(${PROJECT_NAME} ${SOURCES})

# Lua
if (WIN32)
    target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/lua/lib/lua53.dll)
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
add_subdirectory(${LIBRARIES_ROOT}/enet)
target_link_libraries(${PROJECT_NAME} enet)
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
if (APPLE)
    if(CMAKE_BUILD_TYPE MATCHES DEBUG)
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/lowlevel/lib/libfmodF.dylib)
        #target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/studio/lib/libfmodstudioF.dylib)
    else()
        target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/lowlevel/lib/libfmod.dylib)
        #target_link_libraries(${PROJECT_NAME} ${LIBRARIES_ROOT}/fmod/studio/lib/libfmodstudio.dylib)
    endif()
endif()
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/fmod/lowlevel/inc)
#target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/fmod/studio/inc)

# GLM
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/glm)

# sol2
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/sol)

# Imgui
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBRARIES_ROOT}/imgui)

# Symlink assets to an output resources folder
set(SOURCE_RESOURCES ${CMAKE_SOURCE_DIR}/assets)
set(DESTINATION_RESOURCES $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources)

add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_directory ${SOURCE_RESOURCES} ${DESTINATION_RESOURCES}
        DEPENDS ${DESTINATION_RESOURCES})