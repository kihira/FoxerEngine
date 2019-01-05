#define STBI_ONLY_PNG
#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include "AssetManager.h"
#include <fstream>
#include <stb_image.h>
#include <tiny_obj_loader.h>
#include "Managers.h"
#include "render/RenderComponent.h"
#include "Box2D/Collision/Shapes/b2CircleShape.h"
#include "Box2D/Collision/Shapes/b2PolygonShape.h"
#include "Box2D/Dynamics/b2Fixture.h"
#include "entity/EntityManager.h"
#include "InputManager.h"
#include "render/RenderManager.h"
#include "physics/PhysicsManager.h"
#include "event/EventManager.h"
#include "network/NetworkManager.h"
#include "network/NetworkComponent.h"
#include "render/Mesh.h"
#include "render/Shader.h"
#include "entity/Entity.h"
#include "level/Level.h"
#include "render/Material.h"
#include <glm/gtc/type_ptr.inl>
#include "sound/Sample.h"


#define ASSETS_FOLDER "./assets/"
#define ASSETS_EXT ".lua"
#define ERR_SHADER SID("SHADER_ERROR")
#define ERR_MESH SID("MESH_ERROR")
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
    logger = spdlog::get("main")->clone("asset");

    loadDatabase();
    loadStringIds();

    lua.open_libraries(sol::lib::base, sol::lib::math, sol::lib::io);

    sol::table engineTable = gAssetManager.getLua().create_named_table("engine"); // Namespace for interacting with the engine

    // Load functions for lua
    // Can't pass a class instance as the 3rd parameter as it doesn't seem to work with extern

    /*
     * Register entity stuff
     */
    sol::table entityTable = engineTable.create_named("entity");
    engineTable["entity"]["spawnEntity"] = [](const char *id) -> std::shared_ptr<Entity> { return gEntityManager.spawn(processString(id)); };
    engineTable["entity"]["getEntity"] = [](EntityId id) -> std::shared_ptr<Entity> { return gEntityManager.getEntity(id); };
    engineTable["entity"]["destroy"] = [](EntityId id) { gEntityManager.destroy(id); };

    // Register entity type
    entityTable.new_usertype<Entity>(
            "entity",
            "new", sol::factories([](const char *id) -> std::shared_ptr<Entity> { return gEntityManager.spawn(processString(id)); }),
            "id", &Entity::getId,
            // Register properties
            "name", sol::property(&Entity::getName, &Entity::setName),
            "position", sol::property(&Entity::getPosition, &Entity::setPosition),
            "rotation", sol::property(&Entity::getRotation, &Entity::setRotation),
            "getPhysicsComponent", &Entity::getComponent<PhysicsComponent>,
            "getNetworkComponent", &Entity::getComponent<NetworkComponent>,
            sol::meta_function::index, &Entity::dynamicGet,
            sol::meta_function::new_index, &Entity::dynamicSet
    );

    // Input functions
    sol::table inputTable = engineTable.create_named("input");
    engineTable["input"]["registerKeyHandler"] = [](sol::function handler) { gInputManager.registerKeyHandler(handler); };
    engineTable["input"]["registerCursorHandler"] = [](sol::function handler) { gInputManager.registerCursorHandler(handler); };

    /*
     * Register event stuff
     */
    sol::table eventTable = engineTable.create_named("event");
    eventTable["push"] = [](Event &event) { gEventManager.push(event); };

    // Register event type
    eventTable.new_usertype<Event>(
            "event",
            sol::constructors<Event(StringId), Event(const char *)>(),
            "setBool", &Event::setArg<bool>,
            "getBool", &Event::getArg<bool>,
            "setFloat", &Event::setArg<float>,
            "getFloat", &Event::getArg<float>,
            "setInt", &Event::setArg<int>,
            "getInt", &Event::getArg<int>,
            "setShort", &Event::setArg<short>,
            "getShort", &Event::getArg<short>,
            "setUShort", &Event::setArg<unsigned short>,
            "getUShort", &Event::getArg<unsigned short>,
            "setStringId", &Event::setArg<StringId>,
            "getStringId", &Event::getArg<StringId>,
            "setUInt", &Event::setArg<StringId>,
            "getUInt", &Event::getArg<StringId>,
            // Entity ID
            "setEntityId", &Event::setArg<EntityId>,
            "getEntityId", &Event::getArg<EntityId>,
            // Client ID
            "setClientId", &Event::setArg<ClientId>,
            "getClientId", &Event::getArg<ClientId>,
            "type", &Event::getType,
            "push", &Event::push
    );

    /*
     * Register math stuff
     */
    sol::table mathTable = engineTable.create_named("math");

    // Register vec3 type
    mathTable.new_usertype<glm::vec3>(
            "vec3",
            sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
            "x", &glm::vec3::x,
            "y", &glm::vec3::y,
            "z", &glm::vec3::z,
            sol::meta_function::addition, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator+),
            sol::meta_function::subtraction, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator-),
            sol::meta_function::multiplication, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator*),
            sol::meta_function::division, sol::resolve<glm::vec3(const glm::vec3&, const glm::vec3&)>(glm::operator/)
    );

    // Register vec2 type
    mathTable.new_usertype<glm::vec2>(
            "vec2",
            sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
            "x", &glm::vec2::x,
            "y", &glm::vec2::y,
            sol::meta_function::addition, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator+),
            sol::meta_function::subtraction, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator-),
            sol::meta_function::multiplication, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator*),
            sol::meta_function::division, sol::resolve<glm::vec2(const glm::vec2&, const glm::vec2&)>(glm::operator/)
    );

    /*
     * Register network stuff
     */
    sol::table networkTable = engineTable.create_named("network");
    networkTable["isServer"] = []() { return gNetworkManager.isServer(); };
    networkTable["isClient"] = []() { return !gNetworkManager.isServer(); };
    networkTable["clientsCount"] = []() { return gNetworkManager.clientsCount(); };
    networkTable["clientId"] = []() { return gNetworkManager.getClientId(); };
//    networkTable["registerPacket"] = [](PacketId packetId, sol::function callback) {
//        gNetworkManager.registerPacket({
//            packetId,
//            0,
//            ENET_PACKET_FLAG_RELIABLE,
//            ((PacketHandlerFn) &callback) // todo test
//    });
//    };

    // Network component
    networkTable.new_usertype<NetworkComponent>(
            "networkComponent",
            "", sol::no_constructor,
            "hasAuthority", &NetworkComponent::hasAuthority
    );

    /*
     * Register physics stuff
     */
    sol::table physicsTable = engineTable.create_named("physics");
    physicsTable["setGravity"] = [](glm::vec2 &gravity) { gPhysicsManager.setGravity(gravity); };
    physicsTable["getGravity"] = []() -> const glm::vec2 { return gPhysicsManager.getGravity(); };

    // Physics component
    physicsTable.new_usertype<PhysicsComponent>(
            "physicsComponent",
            "", sol::no_constructor,
            "velocity", sol::property(&PhysicsComponent::getVelocity, &PhysicsComponent::setVelocity)
    );

    // Register level type
    engineTable.new_usertype<Level>(
            "level",
            "noconstructor", sol::no_constructor,
            "name", sol::property(&Level::getName, &Level::setName),
            sol::meta_function::index, &Level::dynamicGet,
            sol::meta_function::new_index, &Level::dynamicSet
    );
}

void AssetManager::shutDown() {
    // cleanup();

    delete settings;
}

std::shared_ptr<Mesh> AssetManager::loadMesh(StringId id) {
    if (gNetworkManager.isServer()) {
        return nullptr; // todo temp
    }
    auto it = meshes.find(id);
    if (it != meshes.end()) {
        return it->second;
    }

	// TODO temp
	if (id == ERR_MESH) return getErrorMesh();

    std::string assetData = lua["database"]["meshes"][id];

    // Attempt to load file
    std::ifstream file;
    file.open(ASSETS_FOLDER "meshes/" + assetData, std::ios::in);
    if (!file) {
        logger->error("Error opening mesh file: {}", id);
        return getErrorMesh();
    }

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, (ASSETS_FOLDER "meshes/" + assetData).c_str(), ASSETS_FOLDER "materials/", true) || !err.empty()) {
        logger->error("Error loading obj file: {}", err);
        return getErrorMesh();
    }

	// todo just a quick loader, should improve this for the future
	std::vector<Material *> mats;
	for (auto &matDef : materials) {
		auto material = new Material(glm::make_vec3(matDef.ambient), glm::make_vec3(matDef.diffuse), glm::make_vec3(matDef.specular));
		mats.push_back(material);
	}

	// Create surfaces. From what I've seen, materials/indices should be in order
	std::vector<Surface *> surfaces;
	Surface *surface = nullptr;
	auto currMatId = -1;
    for (auto i = 0; i < shapes[0].mesh.material_ids.size(); ++i) {
	    auto matId = shapes[0].mesh.material_ids[i];

		if (currMatId != matId) {
			// Start new surface
			surface = new Surface();
			surface->material = mats[matId];
			surface->offset = i * 3;
			surfaces.push_back(surface);

			currMatId = matId;
		}
		surface->count += 3;
    }

    GLuint vao;
	GLuint vbos[2];
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, &vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);

    // Position
    auto posSize = attrib.vertices.size() * sizeof(float);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBufferData(GL_ARRAY_BUFFER, posSize, &attrib.vertices[0], GL_STATIC_READ);
//
//    // Normal
//    auto normalSize = attrib.normals.size() * sizeof(float);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid *)posSize);
//
//    // Texcoord
//    auto uvSize = attrib.texcoords.size() * sizeof(float);
//    glEnableVertexAttribArray(1);
//    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid *)(posSize + normalSize));
//
//    glBufferData(GL_ARRAY_BUFFER, posSize + normalSize + uvSize, nullptr, GL_STATIC_READ);
//    glBufferSubData(GL_ARRAY_BUFFER, 0, posSize, &attrib.vertices[0]);
//    glBufferSubData(GL_ARRAY_BUFFER, posSize, normalSize, &attrib.normals[0]);
//    glBufferSubData(GL_ARRAY_BUFFER, posSize + normalSize, uvSize, &attrib.texcoords[0]);

    // Indices data
    std::vector<unsigned short> indices;
    indices.reserve(shapes[0].mesh.indices.size());
    for (auto &index : shapes[0].mesh.indices) {
        indices.push_back(index.vertex_index);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

    GLERRCHECK();

	auto mesh = std::make_shared<Mesh>(vao, vbos, GL_TRIANGLES, GL_UNSIGNED_SHORT, surfaces);
    meshes.emplace(id, mesh);
    return mesh;
}

GLuint AssetManager::loadShader(const std::string &fileName, GLenum shaderType) {
    // Load shader source
    std::ifstream file;
    file.open(ASSETS_FOLDER "shaders/" + fileName, std::ios::in);
    if (!file.is_open()) {
        logger->error("Error opening shader file: {}", fileName);
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
        logger->error("Failed to compile shader {}: {}", fileName, errData);
        return 0;
    }

    GLERRCHECK();

    // If we've made it this far, it's loaded and compiled
    return shader;
}

std::shared_ptr<Shader> AssetManager::loadShaderProgram(StringId id) {
    auto it = shaderPrograms.find(id);
    if (it != shaderPrograms.end()) {
        return it->second;
    }

	// TODO temp
	if (id == ERR_SHADER) return getErrorShader();

    sol::table assetData = lua["database"]["shaders"][id];

    // Load base shaders
    GLuint vertShader, fragShader, program;
    vertShader = loadShader(assetData["vertFile"], GL_VERTEX_SHADER);
    fragShader = loadShader(assetData["fragFile"], GL_FRAGMENT_SHADER);

    // Create program and link them together
    program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    // Delete shaders as they're no longer needed
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    // Verify program is good to go
    GLint err;
    GLchar errData[ERR_SIZE];
    glGetProgramiv(program, GL_LINK_STATUS, &err);
    if (!err) {
        glGetProgramInfoLog(program, ERR_SIZE, nullptr, errData);
        logger->error("Failed to link shader {}: {}", id, errData);
        return getErrorShader();
    }
    GLERRCHECK();

    // Create shader object and register uniforms
    auto shader = std::make_shared<Shader>(program);
    shader->registerUniform("model");
    shader->registerUniform("view");
    shader->registerUniform("projection");

    if (assetData["uniforms"] != sol::lua_nil) {
        auto uniforms = assetData["uniforms"].get<std::vector<std::string>>();
        for (auto &uniform : uniforms) {
            shader->registerUniform(uniform);
        }
    }

    shaderPrograms.emplace(id, shader);

    return shader;
}

std::shared_ptr<Mesh> AssetManager::getErrorMesh() {
    auto it = meshes.find(ERR_MESH);
    if (it != meshes.end()) {
        return it->second;
    }

    GLuint vao;
	GLuint vbos[2];
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(2, &vbos[0]);
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof errCubeVertices, errCubeVertices, GL_STATIC_DRAW);
    // Position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), nullptr);
    // Normal
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<GLvoid *>(3 * sizeof(float)));
    // UV
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<GLvoid *>(6 * sizeof(float)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(errCubeIndices), errCubeIndices, GL_STATIC_DRAW);

    GLERRCHECK();

	auto surface = new Surface();
	surface->offset = 0;
	surface->count = 36;
	surface->material = new Material(glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f));
	std::vector<Surface *> surfaces { surface };

	auto mesh = std::make_shared<Mesh>(vao, vbos, GL_TRIANGLES, GL_UNSIGNED_SHORT, surfaces);
    meshes[ERR_MESH] = mesh;

    return mesh;
}

std::shared_ptr<Shader> AssetManager::getErrorShader() {
    auto it = shaderPrograms.find(ERR_SHADER);
    if (it != shaderPrograms.end()) {
        return it->second;
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

std::shared_ptr<Entity> AssetManager::loadEntityPrototype(StringId id) {
    auto it = entityPrototypes.find(id);
    if (it != entityPrototypes.end()) {
        return it->second;
    }

    // Get file from database
    std::string fileName = lua["database"]["entities"][id];

    auto def = glm::vec3(0.f);

    // Load data from lua file and bind functions
    sol::table entityTable = lua.script_file(ASSETS_FOLDER "entities/" + fileName);
    std::string name = entityTable["name"].get_or(fileName);

    auto entity = std::make_shared<Entity>(0, name);

    entity->setUpdateFn(entityTable["update"]);
    entity->setOnSpawnFn(entityTable["onSpawn"]);
    entity->setPosition(entityTable["position"].get_or(def));
    entity->setRotation(entityTable["rotation"].get_or(def));

    // Create Physics Component
    if (entityTable["physicsComponent"] != sol::lua_nil) {
        auto physicsTable = entityTable["physicsComponent"];

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

        // Contact listeners
        sol::function beginContactFn = physicsTable["beginContact"];
        sol::function endContactFn = physicsTable["endContact"];

        auto physicsComponent = new PhysicsComponent(entity, bodyDef, fixtureDef, beginContactFn, endContactFn);
        physicsComponent->setActive(false);
        entity->addComponent<PhysicsComponent>(physicsComponent);
    }

    // Create Render Component
    if (entityTable["renderComponent"] != sol::lua_nil) {
        auto renderTable = entityTable["renderComponent"];
        auto meshId = renderTable["mesh"].get_or(ERR_MESH);
        auto shaderId = renderTable["shader"].get_or(ERR_SHADER);
        auto mesh = loadMesh(meshId);
        auto shader = loadShaderProgram(shaderId);
        auto renderComponent = new RenderComponent(entity, shader, mesh);
        renderComponent->setActive(false);

        entity->addComponent<RenderComponent>(renderComponent);
    }

    // Create network component
    if (entityTable["networkComponent"] != sol::lua_nil) {
        auto networkTable = entityTable["networkComponent"];
        auto syncRate = networkTable["syncRate"].get_or(.1f);
        auto networkComponent = new NetworkComponent(entity, syncRate);
        networkComponent->setActive(false);

        entity->addComponent<NetworkComponent>(networkComponent);
    }

    // Events
    if (entityTable["events"] != sol::lua_nil && entityTable["onEvent"] != sol::lua_nil) {
        std::vector<const char *> eventIds = entityTable["events"].get<std::vector<const char *>>();
        std::vector<StringId> events;
        events.reserve(eventIds.size());
        for (auto &eventId : eventIds) {
            events.emplace_back(processString(eventId));
        }

        entity->setOnEventFn(entityTable["onEvent"]);
        entity->setEvents(events);
    }

    gEntityManager.registerPrototype(id, entity);
    entityPrototypes[id] = entity;
    return entity;
}

std::shared_ptr<Level> AssetManager::loadLevel(StringId id) {
    auto it = levels.find(id);
    if (it != levels.end()) {
        return it->second;
    }

    // Get file from database
    std::string levelFileName = lua["database"]["levels"][id];

    // Load level file
    auto result = lua.script_file(ASSETS_FOLDER "levels/" + levelFileName);
    if (!result.valid()) {
        sol::error err = result;
        logger->error("Failed to run lua file for level {}: {}", levelFileName, err.what());
    }

    // Load data from lua file
    sol::table levelTable = lua["level"];
    auto level = std::make_shared<Level>(id);
    level->setUpdateFn(levelTable["update"]);

    // Load entities
    if (levelTable["entities"] != sol::lua_nil) {
        sol::table entitiesTable = levelTable["entities"];
        for (auto entry : entitiesTable) {
            sol::table entityTable = entry.second;
            StringId prototypeId = entityTable["prototypeId"];
            EntityId entityId = entityTable["entityId"];

            // Spawn entity
            auto entity = gEntityManager.spawn(prototypeId, entityId);
            entity->setPosition(entityTable["position"]);
        }
    }

    // Events
    if (levelTable["events"] != sol::lua_nil && levelTable["onEvent"] != sol::lua_nil) {
        std::vector<const char *> eventIds = levelTable["events"].get<std::vector<const char *>>();
        std::vector<StringId> events;
        events.reserve(eventIds.size());
        for (auto &eventId : eventIds) {
            events.emplace_back(processString(eventId));
        }

        level->setOnEventFn(levelTable["onEvent"]);
        level->setEvents(events);
    }

    levels.insert(std::make_pair(id, level));
    return level;
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

GLuint AssetManager::loadTexture(StringId id) {
    auto it = textures.find(id);
    if (it != textures.end()) {
        return it->second;
    }

    sol::table assetData = lua["database"]["textures"][id];
    std::string textureFile = assetData["file"].get_or(std::string("error.png"));

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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, assetData["wrapU"].get_or(GL_REPEAT));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, assetData["wrapV"].get_or(GL_REPEAT));

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, assetData["minFilter"].get_or(GL_NEAREST));
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, assetData["magFilter"].get_or(GL_LINEAR));

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureData);

    if (assetData["mipmaps"].get_or(true)) {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    GLERRCHECK();

    // Free image data
    stbi_image_free(textureData);

    textures.emplace(id, textureId);
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

    settings->initialLevel = processString(data["initialLevel"].get_or(std::string("mainmenu")).c_str());

    return settings;
}

std::shared_ptr<Sample> AssetManager::loadSound(StringId id) {
	auto it = sounds.find(id);
	if (it != sounds.end()) {
		return it->second;
	}

	if (lua["database"]["sounds"] == sol::lua_nil) {
		logger->error("Attempted to load sound {} however no sounds exist in the database", id);
		return nullptr; // todo blank sound
	}
	auto assetData = lua["database"]["sounds"][id];
	if (assetData == sol::lua_nil)
	{
		logger->error("Attempted to load sound {} but was not able to find it in the database", id);
		return nullptr; // todo blank sound
	}

	auto sample = std::make_shared<Sample>((ASSETS_FOLDER "sounds/" + assetData["file"].get<std::string>()).c_str());
	sounds.emplace(id, sample);
	return sample;
}

void AssetManager::loadStringIds() {
    std::ifstream file;
    file.open(ASSETS_FOLDER "strings.txt");

    if (!file.is_open()) {
        logger->error("Failed to open string ID definitions file");
        return;
    }

    std::string line;
    while (getline(file, line)) {
	    const auto spacePos = line.find(' ');
        if (spacePos == std::string::npos) {
            logger->error("Failed to properly parse string ID line: {}", line);
            continue;
        }

        auto stringValue = line.substr(0, spacePos);
        StringId stringId = std::stoul(line.substr(spacePos, std::string::npos));

        stringIds.emplace(stringId, stringValue);
    }
}

void AssetManager::loadDatabase() {
    lua.script_file(ASSETS_FOLDER "database.lua");

    // Load events into event manager
    if (lua["database"]["events"] != sol::lua_nil) {
        sol::table events = lua["database"]["events"];
        std::map<StringId, EventMeta> eventMetas;

        for (auto &event : events) {
            StringId eventId = event.first.as<StringId>();

            sol::table eventMetaTable = event.second;
            std::vector<std::string> args = eventMetaTable["args"].get<std::vector<std::string>>();
            auto eventMeta = EventMeta();
            for (auto &arg : args) {
                eventMeta.args.push_back(arg);
            }

            eventMetas.emplace(eventId, eventMeta);
        }

        gEventManager.loadEvents(eventMetas);
    }
}
