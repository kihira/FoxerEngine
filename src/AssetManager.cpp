#include "AssetManager.h"
#include <fstream>
#include <iostream>

#define RESOURCES "./resources/"

Mesh *AssetManager::LoadMesh(std::string name) {
    return nullptr;
}

GLuint AssetManager::LoadShader(const std::string &name, GLenum shaderType) {
    if (shaders.find(name) != shaders.end()) {
        return shaders[name];
    }

    // Load shader source
    std::ifstream file;
    file.open(RESOURCES "shaders/" + name + ".glsl", std::ios::in);
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

Shader *AssetManager::LoadShaderProgram(std::string name) {
    // Read shader definition
    std::ifstream file;
    file.open(RESOURCES "shaders/"+name+".txt", std::ios::in);
    if (!file.is_open()) {
        std::cerr << "Error opening shader definition file: " << name << std::endl;
        return nullptr; // todo return default shader program
    }

    // TODO should be more error resilient and maybe parse json/lua?
    // Read first line for vert shader name, second line for frag shader
    std::string vertShaderName, fragShaderName;
    file >> vertShaderName >> fragShaderName;

    file.close();

    // Load base shaders
    GLuint vertShader, fragShader;
    vertShader = LoadShader(vertShaderName, GL_VERTEX_SHADER);
    fragShader = LoadShader(fragShaderName, GL_FRAGMENT_SHADER);

    return nullptr;
}
