
#include <glad/glad.h>
#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.h"
#include "../gl_helper.hpp"

void Shader::use() {
    if (program == 0) {
        std::cerr << "Something has gone awfully, terribly wrong and you should never ever see this message" << std::endl;
        return;
    }
    glUseProgram(program);
    GLERRCHECK();
}

void Shader::registerUniform(std::string name) {
    if (uniforms.find(name) != uniforms.end()) {
        std::cerr << "Already registered a uniform with the name: " << name << std::endl;
        return;
    }

    if (program == 0) {
        std::cerr << "No program is assigned to the shader so no uniforms can be found" << std::endl;
        return;
    }

    GLint uniformLoc = glGetUniformLocation(program, name.c_str());
    if (uniformLoc == -1) {
        std::cerr << "Cannot find uniform '" << name << "' in the program" << std::endl;
        return;
    }

    uniforms.insert(std::pair<std::string, GLuint>(name, uniformLoc));
}

Shader::Shader(GLuint program) : program(program) {

}

template<typename T>
void Shader::setUniform(const std::string &name, T value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    std::cerr << "Unsupported uniform type" << std::endl;
}

template <>
void Shader::setUniform<float>(const std::string &name, float value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniform1f(uniformLoc->second, value);
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::vec2>(const std::string &name, glm::vec2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniform2fv(uniformLoc->second, 1, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::vec3>(const std::string &name, glm::vec3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniform3fv(uniformLoc->second, 1, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::vec4>(const std::string &name, glm::vec4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniform4fv(uniformLoc->second, 1, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat2x2>(const std::string &name, glm::mat2x2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix2fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat2x3>(const std::string &name, glm::mat2x3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix2x3fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat2x4>(const std::string &name, glm::mat2x4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix2x4fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat3x2>(const std::string &name, glm::mat3x2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix3x2fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat3x3>(const std::string &name, glm::mat3x3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix3fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat3x4>(const std::string &name, glm::mat3x4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix3x4fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat4x2>(const std::string &name, glm::mat4x2 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix4x2fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat4x3>(const std::string &name, glm::mat4x3 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix4x3fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}

template <>
void Shader::setUniform<glm::mat4x4>(const std::string &name, glm::mat4x4 value) {
    auto uniformLoc = uniforms.find(name);
    if (uniformLoc == uniforms.end()) {
        std::cerr << "No uniform with the name '" << name << "' is registered" << std::endl;
        return;
    }

    glUniformMatrix4fv(uniformLoc->second, 1, GL_FALSE, glm::value_ptr(value));
    GLERRCHECK();
}
