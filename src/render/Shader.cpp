
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>
#include "Shader.h"
#include "../gl_helper.hpp"
#include "../assert.h"

void Shader::use() {
    glUseProgram(program);
    GLERRCHECK();
}

void Shader::registerUniform(std::string name) {
    if (uniforms.find(name) != uniforms.end()) {
        spdlog::get("main")->warn("Already registered a uniform with the name: {}", name);
        return;
    }

    if (program == 0) {
        spdlog::get("main")->error("No program is assigned to the shader so no uniforms can be found");
        return;
    }

    GLint uniformLoc = glGetUniformLocation(program, name.c_str());
    if (uniformLoc == -1) {
        spdlog::get("main")->error("Cannot find uniform  '{}' in the program", name);
        return;
    }

    uniforms.insert(std::pair<std::string, GLuint>(name, uniformLoc));
}

Shader::Shader(GLuint program) : program(program) {
    ASSERT(program > 0);
}

Shader::~Shader() {
    glDeleteProgram(program);
}

template<typename T>
void Shader::setUniform(const std::string &name, T value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    spdlog::get("renderer")->error("Unsupported uniform type");
}

GLuint Shader::getProgram() const {
    return program;
}

template <>
void Shader::setUniform<float>(const std::string &name, float value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("renderer")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniform1f(uniformLoc->second, value);
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::vec2>(const std::string &name, glm::vec2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniform2fv(uniformLoc->second, 1, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::vec3>(const std::string &name, glm::vec3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniform3fv(uniformLoc->second, 1, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::vec4>(const std::string &name, glm::vec4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniform4fv(uniformLoc->second, 1, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat2x2>(const std::string &name, glm::mat2x2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix2fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat2x3>(const std::string &name, glm::mat2x3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix2x3fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat2x4>(const std::string &name, glm::mat2x4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix2x4fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat3x2>(const std::string &name, glm::mat3x2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix3x2fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat3x3>(const std::string &name, glm::mat3x3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix3fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat3x4>(const std::string &name, glm::mat3x4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix3x4fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat4x2>(const std::string &name, glm::mat4x2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix4x2fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat4x3>(const std::string &name, glm::mat4x3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix4x3fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat4x4>(const std::string &name, glm::mat4x4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        spdlog::get("main")->warn("No uniform with the name '{}' is registered", name);
        return;
    }

    glUniformMatrix4fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}
