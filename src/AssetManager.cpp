#include "AssetManager.h"
#include <fstream>
#include <iostream>
#include "gl_helper.hpp"
#include "assert.h"

#define ASSETS_FOLDER "./assets/"
#define ERR_SHADER "ERROR"
#define ERR_MESH "ERROR"
#define ERR_SIZE 1024

const char *errVertShaderSrc = R"(
#version 330 core
layout(location = 0) in vec3 vPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    gl_Position = projection * view * model * vec4(vPos, 1);
})";

const char *errFragShaderSrc = R"(
#version 330 core
out vec4 fragColour;
void main() {
    fragColour = vec4(1, 0.41, 0.7, 1);
})";

const GLfloat errCubeVertices[] = {
        -1, -1, 1,
        1, -1, 1,
        1, 1, 1,
        -1, 1, 1,
        -1, -1, -1,
        1, -1, -1,
        1, 1, -1,
        -1, 1, -1
};

const GLushort errCubeIndices[] = {
        0, 1, 2, // BACK
        2, 3, 0,
        1, 5, 6, // RIGHT
        6, 2, 1,
        7, 6, 5, // FRONT
        5, 4, 7,
        4, 0, 3, // LEFT
        3, 7, 4,
        4, 5, 1, // BOTTOM
        1, 0, 4,
        3, 2, 6, // TOP
        6, 7, 3
};

std::shared_ptr<Mesh> AssetManager::loadMesh(std::string name) {
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

std::shared_ptr<Shader> AssetManager::loadShaderProgram(std::string name) {
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
    auto shader = std::make_shared<Shader>(program);
    shaderPrograms[name] = shader;

    return shader;
}

std::shared_ptr<Mesh> AssetManager::getErrorMesh() {
    if (meshes.find(ERR_MESH) != meshes.end()) {
        return meshes[ERR_MESH];
    }

    GLuint vao, vboVertices, vboIndices;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vboVertices);
    glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(errCubeVertices), errCubeVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(errCubeIndices), errCubeIndices, GL_STATIC_DRAW);

    auto mesh = std::make_shared<Mesh>(vao, std::vector<GLuint>(vboVertices), 36, GL_TRIANGLES, GL_UNSIGNED_SHORT);
    meshes[ERR_MESH] = mesh;

    return mesh;
}

std::shared_ptr<Shader> AssetManager::getErrorShader() {
    if (shaderPrograms.find(ERR_SHADER) != shaderPrograms.end()) {
        return shaderPrograms[ERR_SHADER];
    }

    GLuint vertShader, fragShader, program;

    // Vertex shader
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &errVertShaderSrc, nullptr);
    glCompileShader(vertShader);

    // Fragment shader
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &errFragShaderSrc, nullptr);
    glCompileShader(fragShader);

    // Create program and attach shaders
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // Don't need shader sources after linked
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    auto shader = std::make_shared<Shader>(program);
    shader->registerUniform("model");
    shader->registerUniform("view");
    shader->registerUniform("projection");
    shaderPrograms[ERR_SHADER] = shader;
    GLERRCHECK();

    return shader;
}

// TODO this technically won't work as we're modifying an iterable
void AssetManager::cleanup() {
    for (auto &mesh : meshes) {
        if (mesh.second.use_count() <= 1) {
            mesh.second.reset();
            meshes.erase(mesh.first);
        }
    }

    for (auto &shader : shaderPrograms) {
        if (shader.second.use_count() <= 1) {
            shader.second.reset();
            shaderPrograms.erase(shader.first);
        }
    }
}

std::shared_ptr<Entity> AssetManager::getEntityPrototype(std::string fileName, std::string tableName) {
    auto loadResult = lua.load_file(ASSETS_FOLDER "lua/" + fileName);
    if (loadResult.status() != sol::load_status::ok) {
        std::cerr << "Failed to load lua file for entity " << fileName << std::endl;
        return nullptr; // todo should return an error entity
    }
    sol::protected_function_result loadRunResult = loadResult();
    if (!loadRunResult.valid()) {
        sol::error err = loadRunResult;
        std::cerr << "Failed to run lua file for entity " << fileName << ": " << std::endl
                  << err.what() << std::endl;
    }

    // Load data from lua file and bind functions
    sol::table entityTable = lua[tableName];
    std::string name = entityTable["name"].get_or(fileName);
    auto entity = std::make_shared<Entity>(0, name);
    entity->setUpdateFn(entityTable["update"]);

    // Attempt to load mesh for entity
    std::string mesh = entityTable["mesh"].get_or(std::string("ERROR"));
    entity->setMesh(loadMesh(mesh));

    entityPrototypes.insert(std::make_pair(tableName, entity));
    return entity;
}

sol::state &AssetManager::getLua() {
    return lua;
}

void AssetManager::startUp() {
    lua.open_libraries(sol::lib::base, sol::lib::io);
}

void AssetManager::shutDown() {
    cleanup();
}
