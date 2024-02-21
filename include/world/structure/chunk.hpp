#ifndef CHUNK_CLASS_HPP
#define CHUNK_CLASS_HPP

#include <iostream>

#include "opengl/VAO.hpp"
#include "opengl/VBO.hpp"
#include "opengl/EBO.hpp"
#include "opengl/shaders.hpp"
#include "opengl/texture.hpp"

#include "world/structure/cube.hpp"
#include "world/structure/mesh.hpp"
#include "world/structure/octree.hpp"

#include "world/utils/vertex.hpp"
#include "world/utils/texture_coord_cube.hpp"

#include "settings.hpp"

extern Vertex xp;
extern Vertex xm;
extern Vertex yp;
extern Vertex ym;
extern Vertex zp;
extern Vertex zm;

class Chunk
{
    private:
        glm::vec2 pos;
        Octree octree;
        Mesh mesh;

        VAO vao;
        VBO vbo;
        EBO ebo;

        Chunk* chunk_xp;
        Chunk* chunk_xm;
        Chunk* chunk_yp;
        Chunk* chunk_ym;
        
        glm::vec3 getWorldCoord(glm::vec3 pos);

        bool border;

        bool bufferGenerated;

        void addVertices(glm::vec3 pos, Vertex v, TextureCoordFace t);
        void addNeighborFace(Cube* neighbor, Vertex vertex, TextureCoordFace t);

        void deleteCubeMesh(glm::vec3 pos);
        int removeCubeMesh(bool xpB, bool xmB, bool ypB, bool ymB, bool zpB, bool zmB, int i, glm::vec3 coord);
        void updateIndices(int i);
        void removeNeighbor(Chunk* chunk, Cube* neighborCube, Vertex vertex);

        void updateMesh(Node* node);
        void updateBuffer();

        void generateBuffer();

    public:

        void updateMesh();

        Chunk();
        Chunk(glm::vec2 pos, Chunk* chunk_xp, Chunk* chunk_xm, Chunk* chunk_yp, Chunk* chunk_ym);

        bool add(Cube cube);
        Cube* get(glm::vec3 pos);
        Cube remove(glm::vec3 pos);

        Mesh* getMesh();
        glm::vec2 getPosition();

        void draw();

        bool addOctree(Cube cube);

        void removeInMesh(int i);
        bool checkIsFace(int i, Vertex v, glm::vec3 pos);
        bool hasFace(Vertex v, glm::vec3 pos);

        void removeFace(Vertex vertex, glm::vec3 pos);
        void addFace(Vertex v, glm::vec3 pos);

        void setChunkXp(Chunk* chunk);
        void setChunkXm(Chunk* chunk);
        void setChunkYp(Chunk* chunk);
        void setChunkYm(Chunk* chunk);

        void unsetChunkXp();
        void unsetChunkXm();
        void unsetChunkYp();
        void unsetChunkYm();

        Chunk* getChunkXp();
        Chunk* getChunkXm();
        Chunk* getChunkYp();
        Chunk* getChunkYm();
        
        void setBorder();
        void unsetBorder();
        bool isBorder();
        void testBorder();

        void addVerticesFace(Vertex v, int x_min, int y_min, int z_min, int x_max, int y_max, int z_max);
        void unload();
};

#endif