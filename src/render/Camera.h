
#ifndef FOXERENGINE_CAMERA_H
#define FOXERENGINE_CAMERA_H


#include <glm/glm.hpp>

class Camera {
private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::vec3 position;
    glm::vec3 target;
    float fov;
    int width;
    int height;
    // Something else is declaring these as blank preprocessor directives so gotta do this
    float near_;
    float far_;

    void updateViewMatrix();

    void updateProjectionMatrix();
public:
    Camera(const glm::vec3 &position, const glm::vec3 &target, float fov = 90.f, float near = 0.1f, float far = 1000.f);

    void resize(int width, int height);

    void setPosition(const glm::vec3 &position);

    void setTarget(const glm::vec3 &target);

    const glm::mat4 &getProjection() const;

    const glm::mat4 &getView() const;

    /**
     * Gets the cameras current position in world space
     * @return Camera position
     */
    const glm::vec3 &getPosition() const;

    /**
     * Gets the cameras look at target position in world space
     * @return Camera target
     */
    const glm::vec3 &getTarget() const;

    /**
     * Gets the cameras current FOV in degrees
     * @return fov
     */
    float getFov() const;

    /**
     * Sets the cameras FOV in degrees.
     * Clamped between 1 and 179 inclusive
     * @param fov
     */
    void setFov(float fov);
};


#endif //FOXERENGINE_CAMERA_H
