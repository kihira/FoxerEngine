#pragma once
#include <glm/vec3.hpp>

class Shader;

class Material final {
private:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
public:
	Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
		: ambient(ambient),
		  diffuse(diffuse),
		  specular(specular)
	{}

	void apply(Shader *shader) const;
};
