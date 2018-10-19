#ifndef GAMEENGINE301CR_ASSETMANAGER_H
#define GAMEENGINE301CR_ASSETMANAGER_H


#include <string>
#include <glad/glad.h>
#include "render/Mesh.h"
#include "render/Shader.h"

class AssetManager {
private:
    std::map<std::string, GLuint> shaders;
    std::map<std::string, std::shared_ptr<Shader>> shaderPrograms;
    std::map<std::string, std::shared_ptr<Mesh>> meshes;
    std::shared_ptr<Mesh> getErrorMesh();
    std::shared_ptr<Shader> getErrorShader();
public:
    std::shared_ptr<Mesh> loadMesh(std::string name);
    GLuint loadShader(const std::string &name, GLenum shaderType);
    std::shared_ptr<Shader> loadShaderProgram(std::string name);
    /**
     * Goes through all currently loaded assets and removes any that are not currently used.
     *
     * This is done by checking the reference count to each asset and if it is <= 1 (so referenced only from the
     * AssetManager), then it removes it
     */
    void cleanup();
};


#endif //GAMEENGINE301CR_ASSETMANAGER_H
