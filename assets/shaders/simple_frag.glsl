#version 330 core

in vec3 normal;
in vec3 colour;
in vec2 texCoord;

out vec4 fragColour;

void main() {
    fragColour = vec4(colour, 1);
}
