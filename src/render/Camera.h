
#ifndef GAMEENGINE301CR_CAMERA_H
#define GAMEENGINE301CR_CAMERA_H


#include <glm/glm.hpp>

class Camera {
private:
    glm::mat4 projection;
    glm::vec3 position;
    glm::vec3 target;
    float fov;
    float near;
    float far;
public:
    Camera(const glm::vec3 &position, const glm::vec3 &target, float fov = 90.f, float near = 0.1f, float far = 1000.f);
    void resize(int width, int height);
};


#endif //GAMEENGINE301CR_CAMERA_H
