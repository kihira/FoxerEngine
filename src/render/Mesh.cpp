#include <glad/glad.h>
#include <utility>
#include "Mesh.h"
#include "RenderManager.h"
#include "Material.h"

Mesh::Mesh(GLuint vao, GLuint vbos[2], GLenum mode, GLenum indicesType, std::vector<Surface *> surfaces) :
	vao(vao), vbos{vbos[0], vbos[1]}, mode(mode), indicesType(indicesType), surfaces(std::move(surfaces))
{}

Mesh::~Mesh() {
    // Mesh cleans itself up.
    // Assuming that no other meshes use the same VBOs as the AssetManager doesn't even support that
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, &vbos[0]);

	// Cleanup surfaces
	for (auto surface : surfaces)
	{
		delete surface;
		delete surface->material; // todo more of a temp thing until a global material library is set up
	}
}

void Mesh::render(Shader *shader) const {
    glBindVertexArray(vao);

	for (auto surface : surfaces) {
		surface->material->apply(shader);
		glDrawElements(mode, surface->count, indicesType, reinterpret_cast<GLvoid *>(surface->offset * sizeof(unsigned short)));
	}

    GLERRCHECK();
}
