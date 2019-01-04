#include "Light.h"
#include "Shader.h"

Light::Light(const glm::vec3& position, const glm::vec3& direction)
	: position(position),
	  direction(direction)
{}

void Light::apply(Shader* shader) {
	shader->setUniform("light.position", position);
	shader->setUniform("light.direction", direction);
	shader->setUniform("light.intensity", intensity);
}

glm::vec3 Light::getPosition() const {
	return position;
}

void Light::setPosition(const glm::vec3& position) {
	Light::position = position;
}

glm::vec3 Light::getDirection() const {
	return direction;
}

void Light::setDirection(const glm::vec3& direction) {
	Light::direction = direction;
}

float Light::getIntensity() const {
	return intensity;
}

void Light::setIntensity(float intensity) {
	Light::intensity = intensity;
}
