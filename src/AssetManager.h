#ifndef GAMEENGINE301CR_ASSETMANAGER_H
#define GAMEENGINE301CR_ASSETMANAGER_H


#include <string>
#include <glad/glad.h>
#include "Mesh.h"
#include "Shader.h"

class AssetManager {
private:
    std::map<std::string, GLuint> shaders;
    std::map<std::string, GLuint> shaderPrograms;
public:
    Mesh *LoadMesh(std::string name);
    GLuint LoadShader(const std::string &name, GLenum shaderType);
    Shader *LoadShaderProgram(std::string name);
};


#endif //GAMEENGINE301CR_ASSETMANAGER_H
