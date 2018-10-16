#include <glad/glad.h>
#include "Mesh.h"

Mesh::Mesh(GLuint vao, GLuint vertexCount, GLenum mode, GLenum indicesType)
        : vao(vao), vertexCount(vertexCount), mode(mode), indicesType(indicesType) {}

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
}
