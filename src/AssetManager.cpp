#include "AssetManager.h"
#include <fstream>
#include <iostream>

#define ASSETS_FOLDER "./assets/"
#define ERR_SHADER "ERROR"
#define ERR_MESH "ERROR"
#define ERR_SIZE 1024

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

const float errCubeVertices[24] = {
        -1, -1, 1,
        1, -1, 1,
        1, 1, 1,
        -1, 1, 1,
        -1, -1, -1,
        1, -1, -1,
        1, 1, -1,
        -1, 1, -1
};

const short errCubeIndices[36] = {
        0, 1, 2, // BACK
        2, 3, 1,
        1, 5, 6, // RIGHT
        6, 2, 1,
        5, 4, 7, // FRONT
        7, 6, 5,
        4, 0, 3, // LEFT
        3, 7, 4,
        3, 2, 6, // TOP
        6, 7, 3,
        5, 4, 0, // BOTTOM
        0, 1, 5
};

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
    GLchar errData[ERR_SIZE];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(shader, ERR_SIZE, nullptr, errData);
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
    GLuint vertShader, fragShader, program;
    vertShader = loadShader(vertShaderName, GL_VERTEX_SHADER);
    fragShader = loadShader(fragShaderName, GL_FRAGMENT_SHADER);

    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    GLint err;
    GLchar errData[ERR_SIZE];
    glGetProgramiv(program, GL_LINK_STATUS, &err);
    if (!err) {
        glGetProgramInfoLog(program, ERR_SIZE, nullptr, errData);
        std::cerr << "Failed to link shader " << name << ": " << std::endl << errData << std::endl;
        return getErrorShader();
    }

    // Store shader and return
    Shader *shader = new Shader(program);
    shaderPrograms[name] = shader;

    return shader;
}

Mesh *AssetManager::getErrorMesh() {
    if (meshes.find(ERR_MESH) != meshes.end()) {
        return meshes[ERR_MESH];
    }

    GLuint vao, vboVertices, vboIndices;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, 24, &errCubeVertices[0], GL_STATIC_READ);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36, &errCubeIndices[0], GL_STATIC_DRAW);

    Mesh *mesh = new Mesh(vao, {vboVertices, vboIndices}, 12, GL_TRIANGLES, GL_SHORT);
    meshes[ERR_MESH] = mesh;

    return mesh;
}

Shader *AssetManager::getErrorShader() {
    if (shaderPrograms.find(ERR_SHADER) != shaderPrograms.end()) {
        return shaderPrograms[ERR_SHADER];
    }

    return nullptr;
}
