
#ifndef GAMEENGINE301CR_VECTORS_H
#define GAMEENGINE301CR_VECTORS_H

#include <glm/glm.hpp>

namespace Vector {
    const glm::vec3 UP(0.f, 1.f, 0.f);
    const glm::vec3 DOWN(0.f, -1.f, 0.f);
    const glm::vec3 LEFT(-1.f, 0.f, 0.f);
    const glm::vec3 RIGHT(1.f, 1.f, 0.f);
    const glm::vec3 FORWARD(0.f, 0.f, -1.f);
    const glm::vec3 BACKWARD(0.f, 1.f, 1.f);
}

#endif //GAMEENGINE301CR_VECTORS_H
