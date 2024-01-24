#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <GL/glew.h>

struct Mesh
{
    Mesh()
    {
        vertices = std::vector<GLfloat>();
        indices = std::vector<GLuint>();
    };

    void clear() {
        vertices.clear();
        indices.clear();
    }

    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;
};

#endif