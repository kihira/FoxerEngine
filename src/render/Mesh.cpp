#include <utility>

#include <glad/glad.h>
#include "Mesh.h"
#include "../gl_helper.hpp"

Mesh::Mesh(GLuint vao, std::vector<GLuint> vbos, GLuint vertexCount, GLenum mode, GLenum indicesType)
        : vao(vao), vbos(std::move(vbos)), vertexCount(vertexCount), mode(mode), indicesType(indicesType) {}

Mesh::~Mesh() {
    // Mesh cleans itself up.
    // Assuming that no other meshes use the same VBOs as the AssetManager doesn't even support that
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(static_cast<GLsizei>(vbos.size()), &vbos[0]);
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
