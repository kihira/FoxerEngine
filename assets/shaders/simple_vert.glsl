#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(position = 0) in vec3 vPos;
layout(position = 1) in vec3 vNormal;
layout(position = 2) in vec2 vTexCoord;

out vec3 normal;
out vec3 colour;
out vec2 texCoord;

void main() {
    normal = vNormal;
    colour = vColour;
    texCoord = vTexCoord;
    gl_Position = projection * view * model * vec4(vPos, 1);
}