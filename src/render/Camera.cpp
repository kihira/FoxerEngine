
#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(const glm::vec3 &position, const glm::vec3 &target, float fov, float near, float far) :
    position(position), target(target), fov(fov), near_(near), far_(far) {
    updateViewMatrix();
}

void Camera::resize(int width, int height) {
    projection = glm::perspective(glm::radians(fov), (float) width / (float) height, near_, far_);
}

const glm::mat4 &Camera::getProjection() const {
    return projection;
}

const glm::mat4 &Camera::getView() const {
    return view;
}

void Camera::setPosition(const glm::vec3 &position) {
    Camera::position = position;
}

void Camera::setTarget(const glm::vec3 &target) {
    Camera::target = target;
}

void Camera::updateViewMatrix() {
    view = glm::lookAt(position, target, glm::vec3(0.f, 1.f, 0.f));
}
