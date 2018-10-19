#ifndef GAMEENGINE301CR_MESH_H
#define GAMEENGINE301CR_MESH_H

#include <vector>
#include <glad/glad.h>


class Mesh {
private:
    GLuint vao;
    std::vector<GLuint> vbos;
    GLuint vertexCount;
    GLenum mode;
    GLenum indicesType;
public:
    Mesh(GLuint vao, std::vector<GLuint> vbos, GLuint vertexCount, GLenum mode, GLenum indicesType = 0);

    virtual ~Mesh();

    void render();
};


#endif //GAMEENGINE301CR_MESH_H
