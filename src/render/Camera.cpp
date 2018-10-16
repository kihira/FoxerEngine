
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, float fov, float near, float far) :
    position(position), target(target), fov(fov), near(near), far(far) {}

void Camera::resize(int width, int height) {
    projection = glm::perspective(fov, (float) height / width, near, far);
}
