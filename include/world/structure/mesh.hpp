#ifndef MESH_HPP
#define MESH_HPP

#include <mutex>
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

        std::mutex mutex;

        bool is_modified;

        bool checkIsFaceP(int i, Vertex v, glm::vec3 pos);

        void updateIndicesP(int i);

    public:
        Mesh();
        Mesh& operator=(const Mesh& other);

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

        bool needBufferUpdate();
        void bufferUpdate();
};

#endif