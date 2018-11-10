#ifndef GAMEENGINE301CR_ASSETMANAGER_H
#define GAMEENGINE301CR_ASSETMANAGER_H


#include <string>
#include <glad/glad.h>
#include <sol.hpp>
#include "render/Mesh.h"
#include "render/Shader.h"
#include "entity/Entity.h"
#include "level.h"

class AssetManager {
private:
    sol::state lua;
    std::map<std::string, GLuint> shaders;
    std::map<std::string, std::shared_ptr<Shader>> shaderPrograms;
    std::map<std::string, std::shared_ptr<Mesh>> meshes;
    std::map<std::string, std::shared_ptr<Entity>> entityPrototypes;
    std::map<std::string, std::shared_ptr<Level>> levels;

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
public:
    void startUp();

    void shutDown();

    sol::state &getLua();

    /**
     * Retrieves a loaded mesh or attempts to load it from disk
     * @param name
     * @return
     */
    std::shared_ptr<Mesh> loadMesh(std::string name);

    /**
     * Retrieves a loaded shader or attempts to load it from disk
     * @param name
     * @param shaderType
     * @return
     */
    GLuint loadShader(const std::string &name, GLenum shaderType);

    /**
     * Retrieves a shader program or attempts to laod it from disk
     * @param name Name of the program
     * @return The shader program
     */
    std::shared_ptr<Shader> loadShaderProgram(std::string name);

    /**
     * Loads an entity class that can then be spawned later on
     * @param fileName The name of the file EXCLUDING the extension
     * @param tableName The table name of the entity in the file
     * @return The prototype entity that can be used for spawning
     */
    std::shared_ptr<Entity> loadEntityPrototype(std::string fileName, std::string tableName);

    std::shared_ptr<Level> loadLevel(std::string name);

    /**
     * Goes through all currently loaded assets and removes any that are not currently used.
     *
     * This is done by checking the reference count to each asset and if it is <= 1 (so referenced only from the
     * AssetManager), then it removes it
     */
    void cleanup();
};


#endif //GAMEENGINE301CR_ASSETMANAGER_H
