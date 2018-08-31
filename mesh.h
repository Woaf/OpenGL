#ifndef MESH_H
#define MESH_H

#include <GL/glew.h>

class Mesh
{
public:
    Mesh() { VAO = 0; VBO = 0; IBO = 0; indexCount = 0; }

    void CreateMesh(GLfloat* vertices,
                    unsigned int* indices,
                    unsigned int numberOfVertices,
                    unsigned int numberOfIndices);
    void RenderMesh();
    void ClearMesh();

    ~Mesh() { ClearMesh(); }

private:
    GLuint VAO, VBO, IBO;
    GLsizei indexCount;
};

#endif // MESH_H
