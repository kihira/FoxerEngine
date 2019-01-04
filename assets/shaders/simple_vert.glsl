#version 330 core

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(location = 0) in vec3 vPos;
// layout(location = 1) in vec3 vNormal;
// layout(location = 2) in vec2 vTexCoord;

out vec3 normal;
out vec2 texCoord;

void main() {
    // normal = vNormal;
    // texCoord = vTexCoord;
    gl_Position = projection * view * model * vec4(vPos, 1);
}