#ifndef FOXERENGINE_ASSETMANAGER_H
#define FOXERENGINE_ASSETMANAGER_H


#include <string>
#include <glad/glad.h>
#include <sol.hpp>
#include <spdlog/spdlog.h>
#include <map>
#include "Settings.h"
#include "util/StringId.h"

class Mesh;
class Shader;
class Entity;
class Level;
class Sample;

class AssetManager {
private:
    std::shared_ptr<spdlog::logger> logger;
    sol::state lua;
    Settings *settings;
    std::map<StringId, GLuint> textures;
    std::map<StringId, std::shared_ptr<Shader>> shaderPrograms;
    std::map<StringId, std::shared_ptr<Mesh>> meshes;
    std::map<StringId, std::shared_ptr<Entity>> entityPrototypes;
    std::map<StringId, std::shared_ptr<Level>> levels;
    std::map<StringId, std::string> stringIds; // todo move into StringId?
	std::map<StringId, std::shared_ptr<Sample>> sounds;

    /**
     * Returns a default cube mesh that is used to show there is an error
     * @return
     */
    std::shared_ptr<Mesh> getErrorMesh();

    /**
     * Returns a default shader that is used to show there is an error
     * @return
     */
    std::shared_ptr<Shader> getErrorShader();

    void loadDatabase();

    void loadStringIds();
public:
    AssetManager();

    ~AssetManager();

    /**
     * Initialises the AssetManager
     */
    void startUp();

    /**
     * De-initialises the AssetManager
     */
    void shutDown();

    sol::state &getLua();

    /**
     * Retrieves a loaded mesh or attempts to load it from disk
     * @param id ID of the mesh asset
     * @return The mesh
     */
    std::shared_ptr<Mesh> loadMesh(StringId id);

    /**
     * Loads a shader from disk and compules it
     * @param fileName Name of the shader file
     * @param shaderType Type of shader, Vertex or Fragment
     * @return The compiled shader
     */
    GLuint loadShader(const std::string &fileName, GLenum shaderType);

    /**
     * Retrieves a shader program or attempts to load it from disk
     * @param id ID of the program asset
     * @return The shader program
     */
    std::shared_ptr<Shader> loadShaderProgram(StringId id);

    /**
     * Loads an entity class that can then be spawned later on
     * @param id ID of the entity asset
     * @return The prototype entity that can be used for spawning
     */
    std::shared_ptr<Entity> loadEntityPrototype(StringId id);

    std::shared_ptr<Level> loadLevel(StringId id);

	/**
	 * Loads a texture and returns it's OpenGL texture unit
	 * TODO return a texture object instead?
	 */
    GLuint loadTexture(StringId name);

    Settings *loadSettings();

	std::shared_ptr<Sample> loadSound(StringId id);

    /**
     * Goes through all currently loaded assets and removes any that are not currently used.
     *
     * This is done by checking the reference count to each asset and if it is <= 1 (so referenced only from the
     * AssetManager), then it removes it
     */
    void cleanup();
};


#endif //FOXERENGINE_ASSETMANAGER_H
