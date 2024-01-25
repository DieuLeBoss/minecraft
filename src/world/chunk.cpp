#include "world/chunk.hpp"

Vertex xp = Vertex(1.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f);
Vertex xm = Vertex(0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f);
Vertex yp = Vertex(0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f);
Vertex ym = Vertex(0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);
Vertex zp = Vertex(1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f);
Vertex zm = Vertex(0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.0f);

Chunk::Chunk(glm::vec2 ppos, std::vector<TextureCoordCube>* pTextCoord, Chunk* chunk_xp, Chunk* chunk_xm, Chunk* chunk_yp, Chunk* chunk_ym)
{
    pos = ppos;
    octree = Octree();
    mesh = Mesh();

    textCoord = pTextCoord;

    vao = VAO();
    vbo = VBO();
    ebo = EBO();

    vao.LinkVBO(&vbo, 0, 3, GL_FLOAT, 5 * sizeof(float), (void*)0);
    vao.LinkVBO(&vbo, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    vao.Unbind();

    this->chunk_xp = chunk_xp;
    this->chunk_xm = chunk_xm;
    this->chunk_yp = chunk_yp;
    this->chunk_ym = chunk_ym;

}

glm::vec3 Chunk::getWorldCoord(glm::vec3 pos)
{
    return glm::vec3(this->pos.x*CHUNK_X_SIZE+pos.x,
                     pos.y,
                     this->pos.y*CHUNK_Z_SIZE+pos.z);
}

int Chunk::removeCubeMesh(bool xpB, bool xmB, bool ypB, bool ymB, bool zpB, bool zmB, int i, glm::vec3 coord)
{
    int count = 0;

    if(xpB && checkIsFace(i, xp, coord)) {
        removeInMesh(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(xmB && checkIsFace(i, xm, coord)) {
        removeInMesh(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(ypB && checkIsFace(i, yp, coord)) {
        removeInMesh(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(ymB && checkIsFace(i, ym, coord)) {
        removeInMesh(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(zpB && checkIsFace(i, zp, coord)) {
        removeInMesh(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(zmB && checkIsFace(i, zm, coord)) {
        removeInMesh(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    
    return count;
}

void Chunk::deleteCubeMesh(glm::vec3 pos)
{
    int count = 0;

    glm::vec3 coord = getWorldCoord(pos);

    bool xpB = octree.get(glm::vec3(pos.x+1, pos.y, pos.z)) == nullptr;
    bool xmB = octree.get(glm::vec3(pos.x-1, pos.y, pos.z)) == nullptr;
    bool ypB = octree.get(glm::vec3(pos.x, pos.y+1, pos.z)) == nullptr;
    bool ymB = octree.get(glm::vec3(pos.x, pos.y-1, pos.z)) == nullptr;
    bool zpB = octree.get(glm::vec3(pos.x, pos.y, pos.z+1)) == nullptr;
    bool zmB = octree.get(glm::vec3(pos.x, pos.y, pos.z-1)) == nullptr;

    for(int i = 0; i < mesh.vertices.size(); i+=20) {
        count += removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord);

        if(count == 6)
            break;
    }
}

void Chunk::removeInMesh(int i)
{    
    int j = (i / 20) * 6;

    mesh.vertices.erase(mesh.vertices.begin()+i, mesh.vertices.begin()+i+20);
    mesh.indices.erase(mesh.indices.begin()+j, mesh.indices.begin()+j+6);
    
    if(mesh.indices.size() > 0)
        updateIndices(j);
}

void Chunk::updateIndices(int i)
{
    for(int k = i; k < mesh.indices.size(); k++) {
        mesh.indices.at(k) = mesh.indices.at(k)-4;
    }
}

bool Chunk::checkIsFace(int i, Vertex v, glm::vec3 pos)
{
    if(i >= mesh.vertices.size())
        return false;
    return (pos.x + v.v[0][0] == mesh.vertices.at(i+0)) && (pos.y + v.v[0][1] == mesh.vertices.at(i+1)) && (pos.z + v.v[0][2] == mesh.vertices.at(i+2)) &&
           (pos.x + v.v[1][0] == mesh.vertices.at(i+5)) && (pos.y + v.v[1][1] == mesh.vertices.at(i+6)) && (pos.z + v.v[1][2] == mesh.vertices.at(i+7)) &&
           (pos.x + v.v[2][0] == mesh.vertices.at(i+10)) && (pos.y + v.v[2][1] == mesh.vertices.at(i+11)) && (pos.z + v.v[2][2] == mesh.vertices.at(i+12)) &&
           (pos.x + v.v[3][0] == mesh.vertices.at(i+15)) && (pos.y + v.v[3][1] == mesh.vertices.at(i+16)) && (pos.z + v.v[3][2] == mesh.vertices.at(i+17));
}

bool Chunk::add(Cube cube)
{
    if(!octree.add(cube))
        return false;

    glm::vec3 cubeCoord = getWorldCoord(cube.pos);

    TextureCoordCube temp = textCoord->at(cube.id);

    Cube* xpC = octree.get(glm::vec3(cube.pos.x + 1, cube.pos.y, cube.pos.z));
    Cube* xpC_neighbor = nullptr;
    if(cube.pos.x+1 == CHUNK_X_SIZE && chunk_xp != nullptr) {
        xpC_neighbor = chunk_xp->get(glm::vec3(0, cube.pos.y, cube.pos.z));
    }
    if(xpC == nullptr && xpC_neighbor == nullptr) {
        addVertices(cubeCoord, xp, temp.side);
    }


    Cube* xmC = octree.get(glm::vec3(cube.pos.x - 1, cube.pos.y, cube.pos.z));
    Cube* xmC_neighbor = nullptr;
    if(cube.pos.x == 0 && chunk_xm != nullptr) {
        xmC_neighbor = chunk_xm->get(glm::vec3(CHUNK_X_SIZE-1, cube.pos.y, cube.pos.z));
    }
    if(xmC == nullptr && xmC_neighbor == nullptr) {
        addVertices(cubeCoord, xm, temp.side);
    }


    Cube* ypC = octree.get(glm::vec3(cube.pos.x, cube.pos.y + 1, cube.pos.z));
    if(ypC == nullptr) {
        addVertices(cubeCoord, yp, temp.top);
    }


    Cube* ymC = octree.get(glm::vec3(cube.pos.x, cube.pos.y - 1, cube.pos.z));
    if(ymC == nullptr) {
        addVertices(cubeCoord, ym, temp.bottom);
    }


    Cube* zpC = octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z + 1));
    Cube* zpC_neighbor = nullptr;
    if(cube.pos.z+1 == CHUNK_Z_SIZE && chunk_yp != nullptr) {
        zpC_neighbor = chunk_yp->get(glm::vec3(cube.pos.x, cube.pos.y, 0));
    }
    if(zpC == nullptr && zpC_neighbor == nullptr) {
        addVertices(cubeCoord, zp, temp.side);
    }

    
    Cube* zmC = octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z - 1));
    Cube* zmC_neighbor = nullptr;
    if(cube.pos.z == 0 && chunk_ym != nullptr) {
        zmC_neighbor = chunk_ym->get(glm::vec3(cube.pos.x, cube.pos.y, CHUNK_Z_SIZE-1));
    }
    if(zmC == nullptr && zmC_neighbor == nullptr) {
        addVertices(cubeCoord, zm, temp.side);
    }

    int count = 0;
    
    for(int i = 0; i < mesh.vertices.size(); i+=20) {
        if(xmC && checkIsFace(i, xp, getWorldCoord(xmC->pos))) {
            removeInMesh(i);
            count++;
        }
        if(xpC && checkIsFace(i, xm, getWorldCoord(xpC->pos))) {
            removeInMesh(i);
            count++;
        }
        if(ymC && checkIsFace(i, yp, getWorldCoord(ymC->pos))) {
            removeInMesh(i);
            count++;
        }
        if(ypC && checkIsFace(i, ym, getWorldCoord(ypC->pos))) {
            removeInMesh(i);
            count++;
        }
        if(zmC && checkIsFace(i, zp, getWorldCoord(zmC->pos))) {
            removeInMesh(i);
            count++;
        }
        if(zpC && checkIsFace(i, zm, getWorldCoord(zpC->pos))) {
            removeInMesh(i);
            count++;
        }

        if(count == 6)
            break;
    }

    if(chunk_xm != nullptr && xmC_neighbor != nullptr) {
        chunk_xm->removeFace(xp, xmC_neighbor);
    }
    if(chunk_xp != nullptr && xpC_neighbor != nullptr) {
        chunk_xp->removeFace(xm, xpC_neighbor);
    }
    if(chunk_ym != nullptr && zmC_neighbor != nullptr) {
        chunk_ym->removeFace(zp, zmC_neighbor);
    }
    if(chunk_yp != nullptr && zpC_neighbor != nullptr) {
        chunk_yp->removeFace(zm, zpC_neighbor);
    }
    
    updateBuffer();

    return true;
}

void Chunk::removeFace(Vertex vertex, Cube* cube) {
    for(int i = 0; i < mesh.vertices.size(); i+=20) {
        if(checkIsFace(i, vertex, getWorldCoord(cube->pos))) {
            removeInMesh(i);
            updateBuffer();
            break;
        }
    }

}

Cube Chunk::remove(glm::vec3 pos)
{
    Cube temp = octree.remove(pos);

    if(temp.isNull())
        return temp;

    deleteCubeMesh(temp.pos);

    Cube* neighbor;
    glm::vec3 neighborCoord;
    TextureCoordCube texture;

    neighbor = octree.get(glm::vec3(temp.pos.x+1, temp.pos.y, temp.pos.z));
    addNeighborFace(neighbor, xm);
    neighbor = octree.get(glm::vec3(temp.pos.x-1, temp.pos.y, temp.pos.z));
    addNeighborFace(neighbor, xp);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y+1, temp.pos.z));
    addNeighborFace(neighbor, ym);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y-1, temp.pos.z));
    addNeighborFace(neighbor, yp);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y, temp.pos.z+1));
    addNeighborFace(neighbor, zm);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y, temp.pos.z-1));
    addNeighborFace(neighbor, zp);

    updateBuffer();
    
    return temp;
}

void Chunk::addNeighborFace(Cube* neighbor, Vertex vertex)
{
    if(neighbor != nullptr) {
        glm::vec3 neighborCoord = getWorldCoord(neighbor->pos);
        TextureCoordCube texture = textCoord->at(neighbor->id);
        addVertices(neighborCoord, vertex, texture.side);
    }
}

void Chunk::addVertices(glm::vec3 pos, Vertex v, TextureCoordFace t)
{
    mesh.vertices.insert(mesh.vertices.end(), {
        pos.x + v.v[0][0], pos.y + v.v[0][1], pos.z + v.v[0][2], t.v[0][0], t.v[0][1],
        pos.x + v.v[1][0], pos.y + v.v[1][1], pos.z + v.v[1][2], t.v[1][0], t.v[1][1],
        pos.x + v.v[2][0], pos.y + v.v[2][1], pos.z + v.v[2][2], t.v[2][0], t.v[2][1],
        pos.x + v.v[3][0], pos.y + v.v[3][1], pos.z + v.v[3][2], t.v[3][0], t.v[3][1]
    });

    GLuint size = static_cast<GLuint>(mesh.vertices.size()-20)/5;

    mesh.indices.push_back(size+0);
    mesh.indices.push_back(size+1);
    mesh.indices.push_back(size+2);
    mesh.indices.push_back(size+0);
    mesh.indices.push_back(size+2);
    mesh.indices.push_back(size+3);
}

Cube* Chunk::get(glm::vec3 pos)
{
    return octree.get(pos);
}

bool Chunk::addOctree(Cube cube)
{
    return octree.add(cube);
}

void Chunk::updateMesh()
{
    mesh.clear();

    updateMesh(octree.getRoot());

    updateBuffer();
}

void Chunk::updateMesh(Node* node)
{
    if(node->childs[0] != nullptr) {
        for(int i = 0; i < 8; i++) {
            updateMesh(node->childs[i]);
        }
        return;
    }

    Cube cube;

    for(int i = 0; i < node->cubes.size(); i++) {
        cube = node->cubes[i];

        glm::vec3 cubeCoord = getWorldCoord(cube.pos);

        TextureCoordCube temp = textCoord->at(cube.id);

        if(octree.get(glm::vec3(cube.pos.x + 1, cube.pos.y, cube.pos.z)) == nullptr) 
            addVertices(cubeCoord, xp, temp.side);
        
        if(octree.get(glm::vec3(cube.pos.x - 1, cube.pos.y, cube.pos.z)) == nullptr) 
            addVertices(cubeCoord, xm, temp.side);
        
        if(octree.get(glm::vec3(cube.pos.x, cube.pos.y + 1, cube.pos.z)) == nullptr) 
            addVertices(cubeCoord, yp, temp.top);
        
        if(octree.get(glm::vec3(cube.pos.x, cube.pos.y - 1, cube.pos.z)) == nullptr) 
            addVertices(cubeCoord, ym, temp.bottom);
        
        if(octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z + 1)) == nullptr) 
            addVertices(cubeCoord, zp, temp.side);
        
        if(octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z - 1)) == nullptr) 
            addVertices(cubeCoord, zm, temp.side);
    }
}

void Chunk::updateBuffer()
{
    vao.Bind();

    vbo.update(mesh.vertices.data(), mesh.vertices.size() * sizeof(GLfloat));
    ebo.update(mesh.indices.data(), mesh.indices.size() * sizeof(GLuint));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}

void Chunk::draw()
{
    vao.Bind();

    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);

    vao.Unbind();
}

Mesh* Chunk::getMesh()
{
    return &mesh;
}

glm::vec2 Chunk::getPosition()
{
    return pos;
}

void Chunk::setChunkXp(Chunk* chunk) {
    chunk_xp = chunk;
}
void Chunk::setChunkXm(Chunk* chunk) {
    chunk_xm = chunk;
}
void Chunk::setChunkYp(Chunk* chunk) {
    chunk_yp = chunk;
}
void Chunk::setChunkYm(Chunk* chunk) {
    chunk_ym = chunk;
}

void Chunk::setBorder() {
    border = true;
}

void Chunk::unsetBorder() {
    border = false;
}

bool Chunk::isBorder() {
    return border;
}









void Chunk::printMeshPart(int i) {
    for(int j = i; j < i+20; j+=5) {
        std::cout << "    ";
        for(int k = j; k < j+5; k++) {
            std::cout << mesh.vertices.at(k) << ", ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Chunk::printMesh() {
    std::cout << "Mesh :" << std::endl;
    for(int i = 0; i < mesh.vertices.size(); i+=20) {
        for(int j = i; j < i+20; j+=5) {
            std::cout << "    ";
            for(int k = j; k < j+5; k++) {
                std::cout << mesh.vertices.at(k) << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

void Chunk::printIndices() {
    std::cout << "Indices :" << std::endl;
    for(int i = 0; i < mesh.indices.size(); i+=6) {
        for(int j = i; j < i+6; j+=3) {
            std::cout << "    ";
            for(int k = j; k < j+3; k++) {
                std::cout << mesh.indices.at(k) << ", ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}