#include "Managers.h"
#include <fstream>
#include <iostream>
#include "gl_helper.hpp"
#include "assert.h"
#include "render/RenderComponent.h"

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

const std::string errorName = "ERROR";

AssetManager::AssetManager() = default; // noop

AssetManager::~AssetManager() = default; // noop

void AssetManager::startUp() {
    lua.open_libraries(sol::lib::base, sol::lib::io);
}

void AssetManager::shutDown() {
    cleanup();
}

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

std::shared_ptr<Entity> AssetManager::loadEntityPrototype(std::string fileName, std::string tableName) {
    if (entityPrototypes.find(tableName) != entityPrototypes.end()) {
        return entityPrototypes[tableName];
    }

    lua.script_file(ASSETS_FOLDER "scripts/" + fileName + ".lua");

    // Load data from lua file and bind functions
    sol::table entityTable = lua[tableName];
    std::string name = entityTable["name"].get_or(fileName);
    auto entity = std::make_shared<Entity>(0, name);
    entity->setUpdateFn(entityTable["update"]);

    // Create Physics Component
    if (entityTable["collider"] != sol::lua_nil) {
        auto physicsTable = entityTable["collider"];

        // todo switch to using a builder/factory?
        b2BodyDef bodyDef;
        bodyDef.active = physicsTable["active"].get_or(true);
        bodyDef.angle = entity->getRotation().y;
        bodyDef.angularDamping = physicsTable["angularDamping"].get_or(0.f);
        bodyDef.bullet = physicsTable["bullet"].get_or(false);
        bodyDef.linearDamping = physicsTable["linearDamping"].get_or(0.f);
        bodyDef.gravityScale = physicsTable["gravityScale"].get_or(1.f);
        bodyDef.type = static_cast<b2BodyType>(physicsTable["type"].get_or(0));
        bodyDef.position.Set(entity->getPosition().x, entity->getPosition().z);
        bodyDef.fixedRotation = physicsTable["fixedRotation"].get_or(false);
        bodyDef.userData = entity.get();
        auto body = gPhysicsManager.createBody(bodyDef);

        // Create fixture def
        b2FixtureDef fixtureDef;
        fixtureDef.userData = entity.get();
        fixtureDef.density = physicsTable["density"].get_or(0.f);
        fixtureDef.friction = physicsTable["friction"].get_or(.2f);
        fixtureDef.restitution = physicsTable["restitution"].get_or(0.f);
        // fixtureDef.isSensor = physicsTable["isSensor"].get_or(false);
        // fixtureDef.filter;

        // Create shape
        int shapeType = physicsTable["shape"].get_or(0);
        switch (shapeType) {
            case 0: {
                b2CircleShape shape;
                shape.m_radius = physicsTable["radius"].get_or(.5f);
                fixtureDef.shape = &shape;
                break;
            }
            case 1: {
                b2PolygonShape shape;
                shape.SetAsBox(physicsTable["halfWidth"].get_or(.5f), physicsTable["halfHeight"].get_or(.5f));
                fixtureDef.shape = &shape;
                break;
            }
            case 2: {
                // todo
//                b2EdgeShape shape;
//                shape.Set();
//                fixtureDef.shape = &shape;
                break;
            }
        }

        auto fixture = body->CreateFixture(&fixtureDef);

        auto physicsComponent = new PhysicsComponent(entity, body, fixture);
        entity->addComponent(physicsComponent);
    }

    // Create Render Component
    if (entityTable["render"] != sol::lua_nil) {
        auto meshId = entityTable["render"]["mesh"].get_or(errorName);
        auto shaderId = entityTable["render"]["shader"].get_or(errorName);
        auto mesh = loadMesh(meshId);
        auto shader = loadShaderProgram(shaderId);
        auto renderComponent = new RenderComponent(entity, shader, mesh);

        entity->addComponent(renderComponent);
    }

    entityPrototypes.insert(std::make_pair(tableName, entity));
    return entity;
}

std::shared_ptr<Level> AssetManager::loadLevel(std::string name) {
    if (levels.find(name) != levels.end()) {
        return levels[name];
    }

    auto result = lua.script_file(ASSETS_FOLDER "levels/" + name + ".lua");
    if (!result.valid()) {
        sol::error err = result;
        std::cerr << "Failed to run lua file for level " << name << ": " << std::endl
                  << err.what() << std::endl;
    }

    // Load data from lua file
    sol::table levelTable = lua["level"];

    auto level = std::make_shared<Level>();
    level->setUpdateFn(levelTable["update"]);

    levels.insert(std::make_pair(name, level));

    return std::shared_ptr<Level>();
}


sol::state &AssetManager::getLua() {
    return lua;
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