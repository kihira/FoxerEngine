#include "Material.h"
#include "Shader.h"

void Material::apply(Shader* shader) const
{
	shader->setUniform("material.ambient", ambient);
	shader->setUniform("material.diffuse", diffuse);
	shader->setUniform("material.specular", specular);
}
