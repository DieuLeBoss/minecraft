#ifndef CHUNK_CLASS_HPP
#define CHUNK_CLASS_HPP

#include <iostream>

#include "opengl/VAO.hpp"
#include "opengl/VBO.hpp"
#include "opengl/EBO.hpp"
#include "opengl/mesh.hpp"
#include "opengl/shaders.hpp"
#include "opengl/texture.hpp"

#include "cube.hpp"
#include "vertex.hpp"
#include "octree.hpp"

#include "settings.hpp"

struct TextureCoordFace
{
    TextureCoordFace() {}

    TextureCoordFace(
        float ax, float ay,
        float bx, float by,
        float cx, float cy,
        float dx, float dy
    ) {
        v[0][0] = ax;
        v[0][1] = ay;
        v[1][0] = bx;
        v[1][1] = by;
        v[2][0] = cx;
        v[2][1] = cy;
        v[3][0] = dx;
        v[3][1] = dy;
    }

    float v[4][2];
};

struct TextureCoordCube
{
    TextureCoordCube() {}

    TextureCoordCube(
        TextureCoordFace ptop,
        TextureCoordFace pside,
        TextureCoordFace pbottom
    ): top(ptop), side(pside), bottom(pbottom) {}

    TextureCoordFace top;
    TextureCoordFace side;
    TextureCoordFace bottom;
};

class Chunk
{
    private:
        glm::vec2 pos;
        Octree octree;
        Mesh mesh;

        VAO vao;
        VBO vbo;
        EBO ebo;

        std::vector<TextureCoordCube>* textCoord;

        Vertex xp;
        Vertex xm;
        Vertex yp;
        Vertex ym;
        Vertex zp;
        Vertex zm;

        Chunk* chunk_xp;
        Chunk* chunk_xm;
        Chunk* chunk_yp;
        Chunk* chunk_ym;
        
        glm::vec3 getWorldCoord(glm::vec3 pos);

        bool border;

        void addVertices(glm::vec3 pos, Vertex v, TextureCoordFace t);
        void addNeighborFace(Cube* neighbor, Vertex vertex);

        void deleteCubeMesh(glm::vec3 pos);
        int removeCubeMesh(bool xpB, bool xmB, bool ypB, bool ymB, bool zpB, bool zmB, int i, glm::vec3 coord);
        void updateIndices(int i);
        void removeNeighbor(Chunk* chunk, Cube* neighborCube, Vertex vertex);

        void updateMesh(Node* node);
        void updateBuffer();

    public:

        void updateMesh();

        Chunk(glm::vec2 pos, std::vector<TextureCoordCube>* pTextCoord, Chunk* chunk_xp, Chunk* chunk_xm, Chunk* chunk_yp, Chunk* chunk_ym);

        bool add(Cube cube);
        Cube* get(glm::vec3 pos);
        Cube remove(glm::vec3 pos);

        Mesh* getMesh();
        glm::vec2 getPosition();

        void draw();

        bool addOctree(Cube cube);

        void removeInMesh(int i);
        bool checkIsFace(int i, Vertex v, glm::vec3 pos);

        void test(Vertex vertex, Cube* cube);

        void setChunkXp(Chunk* chunk);
        void setChunkXm(Chunk* chunk);
        void setChunkYp(Chunk* chunk);
        void setChunkYm(Chunk* chunk);
        
        void setBorder();
        void unsetBorder();
        bool isBorder();
};

#endif