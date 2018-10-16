#include "AssetManager.h"
#include <fstream>
#include <iostream>

#define ASSETS_FOLDER "./assets/"
#define ERR_SHADER "ERROR"
#define ERR_MESH "ERROR"

const char *errVertShaderSrc = R"(
#version 330 core
uniform mat3 MVP;
layout(position = 0) in vec3 vPos;
void main() {
    gl_Position = vec4(MVP * vPos, 1);
}
)";

const char *errFragShaderSrc = R"(
#version 330 core
out vec4 fragColour;
void main() {
    fragColour = vec4(1, 0.41, 0.7, 1);
}
)";

// TODO default fallback mesh
Mesh *AssetManager::loadMesh(std::string name) {
    if (meshes.find(name) != meshes.end()) {
        return meshes[name];
    }

    // Attempt to load file
    std::ifstream file;
    file.open(ASSETS_FOLDER "meshes/" + name + ".obj", std::ios::in);
    if (!file) {
        std::cerr << "Error opening mesh file: " << name << std::endl;
        return getErrorMesh();
    }

    return nullptr;
}

GLuint AssetManager::loadShader(const std::string &name, GLenum shaderType) {
    if (shaders.find(name) != shaders.end()) {
        return shaders[name];
    }

    // Load shader source
    std::ifstream file;
    file.open(ASSETS_FOLDER "shaders/" + name + ".glsl", std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error opening shader file: " << name << std::endl;
        return 0;
    }

    std::string shaderSource;
    getline(file, shaderSource, '\0');

    GLuint shader = glCreateShader(shaderType);
    // todo glShaderSource(shader, 1, &shaderSource[0], nullptr);
    glCompileShader(shader);

    // Check if the shader has compiled correctly, means its valid code
    GLint err;
    GLchar errData[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(shader, 512, nullptr, errData);
        std::cerr << "Failed to compile shader " << name << ": " << errData << std::endl;
        return 0;
    }

    // If we've made it this far, it's loaded and compiled so we'll save it
    shaders[name] = shader;
    return shader;
}

Shader *AssetManager::loadShaderProgram(std::string name) {
    // Read shader definition
    std::ifstream file;
    file.open(ASSETS_FOLDER "shaders/"+name+".txt", std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error opening shader definition file: " << name << std::endl;
        return getErrorShader();
    }

    // Read first line for vert shader name, second line for frag shader
    std::string vertShaderName, fragShaderName;
    file >> vertShaderName >> fragShaderName;

    file.close();

    // Load base shaders
    GLuint vertShader, fragShader;
    vertShader = loadShader(vertShaderName, GL_VERTEX_SHADER);
    fragShader = loadShader(fragShaderName, GL_FRAGMENT_SHADER);

    return nullptr;
}

Mesh *AssetManager::getErrorMesh() {
    if (meshes.find(ERR_MESH) != meshes.end()) {
        return meshes[ERR_MESH];
    }
    return nullptr;
}

Shader *AssetManager::getErrorShader() {
    if (shaderPrograms.find(ERR_SHADER) != shaderPrograms.end()) {
        return shaderPrograms[ERR_SHADER];
    }

    return nullptr;
}
