#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION

#include "Managers.h"
#include <fstream>
#include <stb_image.h>
#include "assert.h"
#include "render/RenderComponent.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "AssetManager.h"


#define ASSETS_FOLDER "./assets/"
#define ASSETS_EXT ".lua"
#define ERR_SHADER "ERROR"
#define ERR_MESH "ERROR"
#define ERR_SIZE 1024

#define ERR_STR_LOAD_ASSET "Failed to load asset definition for {}"

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
        -1, -1, 1,  1, 1, 1,     0, 0,
        1, -1, 1,   1, 1, 1,     0, 0,
        1, 1, 1,    1, 1, 1,     0, 0,
        -1, 1, 1,   1, 1, 1,     0, 0,
        -1, -1, -1, 1, 1, 1,     0, 0,
        1, -1, -1,  1, 1, 1,     0, 0,
        1, 1, -1,   1, 1, 1,     0, 0,
        -1, 1, -1,  1, 1, 1,     0, 0
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

AssetManager::AssetManager() = default; // noop

AssetManager::~AssetManager() = default; // noop

void AssetManager::startUp() {
    logger = spdlog::stdout_color_mt("asset");

    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io);
}

void AssetManager::shutDown() {
    // cleanup();

    delete settings;
}

std::shared_ptr<Mesh> AssetManager::loadMesh(std::string name) {
    if (meshes.find(name) != meshes.end()) {
        return meshes[name];
    }

    auto result = lua.script_file(ASSETS_FOLDER "meshes/" + name + ASSETS_EXT);
    if (!result.valid()) {
        logger->error(ERR_STR_LOAD_ASSET, name);
        return getErrorMesh();
    }
    sol::table assetData = result;

    if (assetData["texture"] != sol::lua_nil) {
        // todo use texture somehow
        loadTexture(assetData["texture"]);
    }

    // Attempt to load file
    std::ifstream file;
    file.open(ASSETS_FOLDER "meshes/" + assetData["file"].get_or(std::string(ERR_MESH)), std::ios::in);
    if (!file) {
        logger->error("Error opening mesh file: {}", name);
        return getErrorMesh();
    }

    std::vector<glm::vec3> normals;
    std::vector<glm::vec3> vertices;

    std::string line;
    while (getline(file, line)) {
        if (line[0] == 'v' && line[1] == 'n') { // normal
            glm::vec3 normal;
            char *next;
            float t = strtof(line.c_str(), &next);
            sscanf(line.c_str(), "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            normals.push_back(normal);
        } else if (line[0] == 'v' && line[1] == 't') { // texture coord

        } else if (line[0] == 'v') { // position
            glm::vec3 vertex;
            sscanf(line.c_str(), "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            vertices.push_back(vertex);
        } else if (line[0] == 'f') { // Face

        }
    }

    GLuint vao, vertexBuffer, indicesBuffer;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vertexBuffer);
    glGenBuffers(1, &indicesBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    GLsizei vertexSize = sizeof(glm::vec3) + sizeof(glm::vec3) + sizeof(glm::vec2);

    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, sizeof(glm::vec3), GL_FLOAT, GL_FALSE, vertexSize, nullptr);

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizeof(glm::vec3), GL_FLOAT, GL_FALSE, vertexSize, (GLvoid *)sizeof(glm::vec3));

    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, sizeof(glm::vec3), GL_FLOAT, GL_FALSE, vertexSize, (GLvoid *)(sizeof(glm::vec3) + sizeof(glm::vec3)));

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_READ);

    GLERRCHECK();

    auto mesh = std::make_shared<Mesh>(vao, indicesBuffer, vertexBuffer, 0, GL_TRIANGLES, GL_UNSIGNED_SHORT);
    meshes.insert(std::make_pair(name, mesh));
    return mesh;
}

GLuint AssetManager::loadShader(const std::string &name, GLenum shaderType) {
    if (shaders.find(name) != shaders.end()) {
        return shaders[name];
    }

    // Load shader source
    std::ifstream file;
    file.open(ASSETS_FOLDER "shaders/" + name + ".glsl", std::ios::in);
    if (!file.is_open()) {
        logger->error("Error opening shader file: {}", name);
        return 0;
    }

    std::string shaderSource;
    getline(file, shaderSource, '\0');
    const GLchar *shaderSrcPtr = shaderSource.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSrcPtr, nullptr);
    glCompileShader(shader);

    // Check if the shader has compiled correctly, means its valid code
    GLint err;
    GLchar errData[ERR_SIZE];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &err);
    if (!err)
    {
        glGetShaderInfoLog(shader, ERR_SIZE, nullptr, errData);
        logger->error("Failed to compile shader {}: {}", name, errData);
        return 0;
    }

    GLERRCHECK();

    // If we've made it this far, it's loaded and compiled so we'll save it
    shaders[name] = shader;
    return shader;
}

std::shared_ptr<Shader> AssetManager::loadShaderProgram(std::string name) {
    // Read shader definition
    std::ifstream file;
    file.open(ASSETS_FOLDER "shaders/" + name + ASSETS_EXT, std::ios::in);
    if (!file.is_open()) {
        logger->error("Error opening shader definition file: {}", name);
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
        logger->error("Failed to link shader {}: {}", name, errData);
        return getErrorShader();
    }

    GLERRCHECK();

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
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(3 * sizeof(float)));
    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (GLvoid *)(6 * sizeof(float)));

    glGenBuffers(1, &vboIndices);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(errCubeIndices), errCubeIndices, GL_STATIC_DRAW);

    GLERRCHECK();

    auto mesh = std::make_shared<Mesh>(vao, vboIndices, vboVertices, 36, GL_TRIANGLES, GL_UNSIGNED_SHORT);
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

    lua.script_file(ASSETS_FOLDER "entities/" + fileName + ASSETS_EXT);

    // Load data from lua file and bind functions
    sol::table entityTable = lua[tableName];
    std::string name = entityTable["name"].get_or(fileName);
    auto entity = std::make_shared<Entity>(0, name);
    entity->setUpdateFn(entityTable["update"]);
    entity->setOnSpawnFn(entityTable["onSpawn"]);

    // Create Physics Component
    if (entityTable["collider"] != sol::lua_nil) {
        auto physicsTable = entityTable["collider"];

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

        // Create fixture def
        b2FixtureDef fixtureDef;
        fixtureDef.userData = entity.get();
        fixtureDef.density = physicsTable["density"].get_or(0.f);
        fixtureDef.friction = physicsTable["friction"].get_or(.2f);
        fixtureDef.restitution = physicsTable["restitution"].get_or(0.f);
        // fixtureDef.isSensor = physicsTable["isSensor"].get_or(false);
        // fixtureDef.filter;

        // Create shape
        // Create a pointer here so it doesn't drop out of scope for later when cloning
        int shapeType = physicsTable["shape"].get_or(0);
        switch (shapeType) {
            case 0: {
                auto shape = new b2CircleShape();
                shape->m_radius = physicsTable["radius"].get_or(.5f);
                fixtureDef.shape = shape;
                break;
            }
            case 1: {
                auto shape = new b2PolygonShape();
                shape->SetAsBox(physicsTable["halfWidth"].get_or(.5f), physicsTable["halfHeight"].get_or(.5f));
                fixtureDef.shape = shape;
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

        auto physicsComponent = new PhysicsComponent(entity, bodyDef, fixtureDef);
        physicsComponent->setActive(false);
        entity->addComponent(std::type_index(typeid(PhysicsComponent)), physicsComponent);
    }

    // Create Render Component
    if (entityTable["render"] != sol::lua_nil) {
        auto meshId = entityTable["render"]["mesh"].get_or(std::string(ERR_MESH));
        auto shaderId = entityTable["render"]["shader"].get_or(std::string(ERR_SHADER));
        auto mesh = loadMesh(meshId);
        auto shader = loadShaderProgram(shaderId);
        auto renderComponent = new RenderComponent(entity, shader, mesh);
        renderComponent->setActive(false);

        entity->addComponent(std::type_index(typeid(RenderComponent)), renderComponent);
    }

    gEntityManager.registerPrototype(tableName, entity);
    entityPrototypes[tableName] = entity;
    return entity;
}

std::shared_ptr<Level> AssetManager::loadLevel(std::string name) {
    if (levels.find(name) != levels.end()) {
        return levels[name];
    }

    auto result = lua.script_file(ASSETS_FOLDER "levels/" + name + ASSETS_EXT);
    if (!result.valid()) {
        sol::error err = result;
        logger->error("Failed to run lua file for level {}: {}", name, err.what());
    }

    // Load data from lua file
    sol::table levelTable = lua["level"];
    auto level = std::make_shared<Level>();
    level->setUpdateFn(levelTable["update"]);

    // Load entities
    if (levelTable["entities"] != sol::lua_nil) {
        sol::table entitiesTable = levelTable["entities"];
        for (auto i = entitiesTable.begin(); i != entitiesTable.end(); i++) {
            // todo spawn entities
        }
    }

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

GLuint AssetManager::loadTexture(std::string name) {
    if (textures.find(name) != textures.end()) {
        return textures[name];
    }

    auto table = lua.script_file(ASSETS_FOLDER "textures/" + name + ASSETS_EXT);
    if (!table.valid()) {
        logger->error(ERR_STR_LOAD_ASSET, name);
        return 0;
    }
    sol::table data = table;
    std::string textureFile = data["file"].get_or(std::string("error.png"));
    int width, height, channels;
    auto textureData = stbi_load((ASSETS_FOLDER "textures/" + textureFile).c_str(), &width, &height, &channels, 4);

    if (textureData == nullptr) {
        logger->error("Error opening texture file: {}", textureFile);
        return 0;
    }

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Load and set image options
    // todo should do some proper parsing and error checking
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, data["wrapU"].get_or(GL_REPEAT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, data["wrapV"].get_or(GL_REPEAT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, data["minFilter"].get_or(GL_NEAREST));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, data["magFilter"].get_or(GL_LINEAR));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    if (data["mipmaps"].get_or(true)) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    GLERRCHECK();

    // Free image data
    stbi_image_free(textureData);

    textures.insert(std::make_pair(name, textureId));
    return textureId;
}

Settings *AssetManager::loadSettings() {
    if (AssetManager::settings != nullptr) {
        return AssetManager::settings;
    }
    auto settings = new Settings();

    auto table = lua.script_file(ASSETS_FOLDER "settings" ASSETS_EXT);
    if (!table.valid()) {
        logger->error("Failed to load settings file, using defaults");
        return settings;
    }

    sol::table data = table;
    settings->windowTitle = data["window"]["title"].get_or(std::string("Window Title"));
    settings->windowWidth = data["window"]["width"].get_or(1920);
    settings->windowHeight = data["window"]["height"].get_or(1080);

    settings->cameraFov = data["camera"]["fov"].get_or(75.f);

    settings->initialLevel = data["initialLevel"].get_or(std::string("mainmenu"));

    return settings;
}
