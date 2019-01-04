#pragma once
#include <glm/vec3.hpp>

class Shader;

/**
 * A simple directional light
 */
class Light {
private:
	glm::vec3 position;
	glm::vec3 direction;
	float intensity;
public:
	Light(const glm::vec3& position, const glm::vec3& direction);

	/**
	 * Applies the light properties to the shader
	 */
	void apply(Shader *shader);

	glm::vec3 getPosition() const;

	void setPosition(const glm::vec3& position);

	glm::vec3 getDirection() const;

	void setDirection(const glm::vec3& direction);

	float getIntensity() const;

	void setIntensity(float intensity);
};
