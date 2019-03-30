#ifndef FOXERENGINE_MESH_H
#define FOXERENGINE_MESH_H

#include <glad/glad.h>
#include <vector>

class Material;
class Shader;

struct Surface final {
	unsigned short offset;
	unsigned short count;
	Material *material;
};

class Mesh final {
private:
    GLuint vao;
    GLuint vbos[2];
    GLenum mode;
    GLenum indicesType;
	std::vector<Surface *> surfaces;
public:
	Mesh(GLuint vao, GLuint vbos[2], GLenum mode, GLenum indicesType, std::vector<Surface *> surfaces);

    ~Mesh();

    void render(Shader *shader) const;
};


#endif //FOXERENGINE_MESH_H
