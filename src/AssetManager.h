#ifndef GAMEENGINE301CR_ASSETMANAGER_H
#define GAMEENGINE301CR_ASSETMANAGER_H


#include <string>
#include <glad/glad.h>
#include "render/Mesh.h"
#include "render/Shader.h"

class AssetManager {
private:
    std::map<std::string, GLuint> shaders;
    std::map<std::string, Shader *> shaderPrograms;
    std::map<std::string, Mesh *> meshes;
    Mesh *getErrorMesh();
    Shader *getErrorShader();
public:
    Mesh *loadMesh(std::string name);
    GLuint loadShader(const std::string &name, GLenum shaderType);
    Shader *loadShaderProgram(std::string name);
};


#endif //GAMEENGINE301CR_ASSETMANAGER_H
