#version 330 core

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Material material;

in vec3 normal;
in vec2 texCoord;

out vec4 fragColour;

void main() {
	vec3 col = material.ambient + material.specular;
    fragColour = vec4(material.diffuse, 1);
}
