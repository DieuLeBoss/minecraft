#ifndef MESH_HPP
#define MESH_HPP

#include <vector>
#include <glm/glm.hpp>
#include <GL/glew.h>

#include "world/utils/vertex.hpp"

#include "world/utils/texture_coord_face.hpp"

#include "settings.hpp"

class Mesh
{
    private:
        std::vector<GLfloat> vertices;
        std::vector<GLuint> indices;

    public:
        Mesh();

        void addVertices(glm::vec3 pos, Vertex v, TextureCoordFace t);
        void removeVertices(int i);

        bool hasFace(Vertex v, glm::vec3 pos);
        bool checkIsFace(int i, Vertex v, glm::vec3 pos);

        void updateIndices(int i);

        void clear();

        GLfloat* getVerticesData();
        GLuint* getIndicesData();

        unsigned long getVerticesSize();
        unsigned long getIndicesSize();
};

#endif