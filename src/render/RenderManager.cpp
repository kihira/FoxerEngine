
#include "RenderManager.h"

// todo should this just be called from the Shader itself?
void RenderManager::useShader(std::shared_ptr<Shader> shader) {
    if (shader->getProgram() == currentShader) return;

    glUseProgram(shader->getProgram());
}
