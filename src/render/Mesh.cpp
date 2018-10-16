#include <glad/glad.h>
#include "Mesh.h"
#include "../gl_helper.hpp"

Mesh::Mesh(GLuint vao, std::vector<GLuint> vbos, GLuint vertexCount, GLenum mode, GLenum indicesType)
        : vao(vao), vbos(vbos), vertexCount(vertexCount), mode(mode), indicesType(indicesType) {}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &vao);
}

void Mesh::render() {
    glBindVertexArray(vao);
    if (indicesType > 0) {
        glDrawElements(mode, vertexCount, indicesType, nullptr);
    }
    else {
        glDrawArrays(mode, 0, vertexCount);
    }
    GLERRCHECK();
}
