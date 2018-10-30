
#ifndef GAMEENGINE301CR_RENDERMANAGER_H
#define GAMEENGINE301CR_RENDERMANAGER_H


#include <glad/glad.h>
#include "Shader.h"

class RenderManager {
private:
    GLuint currentShader;
public:
    void startUp();

    void shutDown();

    void useShader(std::shared_ptr<Shader> shader);
};


#endif //GAMEENGINE301CR_RENDERMANAGER_H
