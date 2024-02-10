#include "world/structure/chunk.hpp"

Vertex xp = Vertex(1.0f,0.0f,0.0f,1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f);
Vertex xm = Vertex(0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.0f,0.0f,0.0f);
Vertex yp = Vertex(0.0f,1.0f,0.0f,0.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,0.0f);
Vertex ym = Vertex(0.0f,0.0f,0.0f,1.0f,0.0f,0.0f,1.0f,0.0f,1.0f,0.0f,0.0f,1.0f);
Vertex zp = Vertex(1.0f,0.0f,1.0f,1.0f,1.0f,1.0f,0.0f,1.0f,1.0f,0.0f,0.0f,1.0f);
Vertex zm = Vertex(0.0f,0.0f,0.0f,0.0f,1.0f,0.0f,1.0f,1.0f,0.0f,1.0f,0.0f,0.0f);


Chunk::Chunk() {}

Chunk::Chunk(glm::vec2 ppos, Chunk* chunk_xp, Chunk* chunk_xm, Chunk* chunk_yp, Chunk* chunk_ym)
{
    pos = ppos;
    octree = Octree();
    mesh = Mesh();

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

#pragma region "Utils"

glm::vec3 Chunk::getWorldCoord(glm::vec3 pos)
{
    return glm::vec3(this->pos.x*(CHUNK_WIDTH/CHUNK_SCALE)+pos.x/CHUNK_SCALE,
                     pos.y/CHUNK_SCALE,
                     this->pos.y*(CHUNK_WIDTH/CHUNK_SCALE)+pos.z/CHUNK_SCALE);
}

Cube* Chunk::get(glm::vec3 pos)
{
    return octree.get(pos);
}


bool Chunk::addOctree(Cube cube)
{
    return octree.add(cube);
}

void Chunk::updateBuffer()
{
    vao.Bind();

    vbo.update(mesh.getVerticesData(), mesh.getVerticesSize() * sizeof(GLfloat));
    ebo.update(mesh.getIndicesData(), mesh.getIndicesSize() * sizeof(GLuint));

    vao.Unbind();
    vbo.Unbind();
    ebo.Unbind();
}


void Chunk::draw()
{
    vao.Bind();

    glDrawElements(GL_TRIANGLES, mesh.getIndicesSize(), GL_UNSIGNED_INT, 0);

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
    this->chunk_xp = chunk;
}
void Chunk::setChunkXm(Chunk* chunk) {
    this->chunk_xm = chunk;
}
void Chunk::setChunkYp(Chunk* chunk) {
    this->chunk_yp = chunk;
}
void Chunk::setChunkYm(Chunk* chunk) {
    this->chunk_ym = chunk;
}


void Chunk::unsetChunkXp() {
    addVerticesFace(xp, CHUNK_WIDTH-1, 0, 0, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);
    chunk_xp = nullptr;   
}
void Chunk::unsetChunkXm() {
    addVerticesFace(xm, 0, 0, 0, 1, CHUNK_HEIGHT, CHUNK_WIDTH);
    chunk_xm = nullptr;   
}
void Chunk::unsetChunkYp() {
    addVerticesFace(zp, 0, 0, CHUNK_WIDTH-1, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH);
    chunk_yp = nullptr;   
}
void Chunk::unsetChunkYm() {
    addVerticesFace(zm, 0, 0, 0, CHUNK_WIDTH, CHUNK_HEIGHT, 1);
    chunk_ym = nullptr;   
}


Chunk* Chunk::getChunkXp() {
    return chunk_xp;
}
Chunk* Chunk::getChunkXm() {
    return chunk_xm;
}
Chunk* Chunk::getChunkYp() {
    return chunk_yp;
}
Chunk* Chunk::getChunkYm() {
    return chunk_ym;
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

void Chunk::testBorder() {
    if(chunk_xm && chunk_xp && chunk_ym && chunk_yp) {
        unsetBorder();
        return;
    }
    setBorder();
}

void Chunk::unload() {
    if(chunk_xp)
        chunk_xp->unsetChunkXm();

    if(chunk_xm)
        chunk_xm->unsetChunkXp();

    if(chunk_yp) 
        chunk_yp->unsetChunkYm();

    if(chunk_ym)
        chunk_ym->unsetChunkYp();
}

void Chunk::addVerticesFace(Vertex v, int x_min, int y_min, int z_min, int x_max, int y_max, int z_max) {
    for(int x = x_min; x < x_max; x++) {
        for(int y = y_min; y < y_max; y++) {
            for(int z = z_min; z < z_max; z++) {
                addFace(v, glm::vec3(x, y, z));
            }
        }
    }
    updateBuffer();
}


#pragma endregion



#pragma region "Remove"

Cube Chunk::remove(glm::vec3 pos)
{
    Cube temp = octree.remove(pos);

    if(temp.isNull())
        return temp;

    deleteCubeMesh(temp.pos);

    Cube* neighbor;

    neighbor = octree.get(glm::vec3(temp.pos.x+1, temp.pos.y, temp.pos.z));
    if(neighbor)
        addNeighborFace(neighbor, xm, TEXTURE_COORD.at(neighbor->id).side);
    neighbor = octree.get(glm::vec3(temp.pos.x-1, temp.pos.y, temp.pos.z));
    if(neighbor)
        addNeighborFace(neighbor, xp, TEXTURE_COORD.at(neighbor->id).side);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y+1, temp.pos.z));
    if(neighbor)
        addNeighborFace(neighbor, ym, TEXTURE_COORD.at(neighbor->id).bottom);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y-1, temp.pos.z));
    if(neighbor)
        addNeighborFace(neighbor, yp, TEXTURE_COORD.at(neighbor->id).top);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y, temp.pos.z+1));
    if(neighbor)
        addNeighborFace(neighbor, zm, TEXTURE_COORD.at(neighbor->id).side);
    neighbor = octree.get(glm::vec3(temp.pos.x, temp.pos.y, temp.pos.z-1));
    if(neighbor)
        addNeighborFace(neighbor, zp, TEXTURE_COORD.at(neighbor->id).side);
    
    
    if(chunk_xp && pos.x == CHUNK_WIDTH-1) {
        chunk_xp->addFace(xm, glm::vec3(0, pos.y, pos.z));
        chunk_xp->updateBuffer();
    }
    
    if(chunk_xm && pos.x == 0) {
        chunk_xm->addFace(xp, glm::vec3(CHUNK_WIDTH-1, pos.y, pos.z));
        chunk_xm->updateBuffer();
    }

    if(chunk_yp && pos.z == CHUNK_WIDTH-1) {
        chunk_yp->addFace(zm, glm::vec3(pos.x, pos.y, 0));
        chunk_yp->updateBuffer();
    }

    if(chunk_ym && pos.z == 0) {
        chunk_ym->addFace(zp, glm::vec3(pos.x, pos.y, CHUNK_WIDTH-1));
        chunk_ym->updateBuffer();
    }

    updateBuffer();
    
    return temp;
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

    for(int i = 0; i < mesh.getVerticesSize(); i+=20) {
        count += removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord);

        if(count == 6)
            break;
    }
}

int Chunk::removeCubeMesh(bool xpB, bool xmB, bool ypB, bool ymB, bool zpB, bool zmB, int i, glm::vec3 coord)
{
    int count = 0;

    if(xpB && mesh.checkIsFace(i, xp, coord)) {
        mesh.removeVertices(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(xmB && mesh.checkIsFace(i, xm, coord)) {
        mesh.removeVertices(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(ypB && mesh.checkIsFace(i, yp, coord)) {
        mesh.removeVertices(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(ymB && mesh.checkIsFace(i, ym, coord)) {
        mesh.removeVertices(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(zpB && mesh.checkIsFace(i, zp, coord)) {
        mesh.removeVertices(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    if(zmB && mesh.checkIsFace(i, zm, coord)) {
        mesh.removeVertices(i);
        count++;
        return removeCubeMesh(xpB, xmB, ypB, ymB, zpB, zmB, i, coord) + count;
    }
    
    return count;
}

void Chunk::addNeighborFace(Cube* neighbor, Vertex vertex, TextureCoordFace t)
{
    if(neighbor) {
        glm::vec3 neighborCoord = getWorldCoord(neighbor->pos);
        mesh.addVertices(neighborCoord, vertex, t);
    }
}

void Chunk::addFace(Vertex v, glm::vec3 pos) {
    Cube* cube = get(pos);

    if(!cube) {
        return;
    }

    glm::vec3 world_pos = getWorldCoord(pos);
    if(!mesh.hasFace(v, world_pos)) {
        mesh.addVertices(world_pos, v, TEXTURE_COORD.at(cube->id).side);
    }
}



#pragma endregion



#pragma region "Add"

bool Chunk::add(Cube cube)
{
    if(!octree.add(cube))
        return false;

    glm::vec3 cubeCoord = getWorldCoord(cube.pos);

    TextureCoordCube temp = TEXTURE_COORD.at(cube.id);

    Cube* xpC = octree.get(glm::vec3(cube.pos.x + 1, cube.pos.y, cube.pos.z));
    Cube* xpC_neighbor = nullptr;
    if(cube.pos.x+1 == CHUNK_WIDTH && chunk_xp) {
        xpC_neighbor = chunk_xp->get(glm::vec3(0, cube.pos.y, cube.pos.z));
    }
    if(!xpC && !xpC_neighbor) {
        mesh.addVertices(cubeCoord, xp, temp.side);
    }


    Cube* xmC = octree.get(glm::vec3(cube.pos.x - 1, cube.pos.y, cube.pos.z));
    Cube* xmC_neighbor = nullptr;
    if(cube.pos.x == 0 && chunk_xm) {
        xmC_neighbor = chunk_xm->get(glm::vec3(CHUNK_WIDTH-1, cube.pos.y, cube.pos.z));
    }
    if(!xmC && !xmC_neighbor) {
        mesh.addVertices(cubeCoord, xm, temp.side);
    }


    Cube* ypC = octree.get(glm::vec3(cube.pos.x, cube.pos.y + 1, cube.pos.z));
    if(!ypC) {
        mesh.addVertices(cubeCoord, yp, temp.top);
    }


    Cube* ymC = octree.get(glm::vec3(cube.pos.x, cube.pos.y - 1, cube.pos.z));
    if(!ymC) {
        mesh.addVertices(cubeCoord, ym, temp.bottom);
    }


    Cube* zpC = octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z + 1));
    Cube* zpC_neighbor = nullptr;
    if(cube.pos.z+1 == CHUNK_WIDTH && chunk_yp) {
        zpC_neighbor = chunk_yp->get(glm::vec3(cube.pos.x, cube.pos.y, 0));
    }
    if(!zpC && !zpC_neighbor) {
        mesh.addVertices(cubeCoord, zp, temp.side);
    }

    
    Cube* zmC = octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z - 1));
    Cube* zmC_neighbor = nullptr;
    if(cube.pos.z == 0 && chunk_ym) {
        zmC_neighbor = chunk_ym->get(glm::vec3(cube.pos.x, cube.pos.y, CHUNK_WIDTH-1));
    }
    if(!zmC && !zmC_neighbor) {
        mesh.addVertices(cubeCoord, zm, temp.side);
    }

    int count = 0;
    
    for(int i = 0; i < mesh.getVerticesSize(); i+=20) {
        if(xmC && mesh.checkIsFace(i, xp, getWorldCoord(xmC->pos))) {
            mesh.removeVertices(i);
            count++;
        }
        if(xpC && mesh.checkIsFace(i, xm, getWorldCoord(xpC->pos))) {
            mesh.removeVertices(i);
            count++;
        }
        if(ymC && mesh.checkIsFace(i, yp, getWorldCoord(ymC->pos))) {
            mesh.removeVertices(i);
            count++;
        }
        if(ypC && mesh.checkIsFace(i, ym, getWorldCoord(ypC->pos))) {
            mesh.removeVertices(i);
            count++;
        }
        if(zmC && mesh.checkIsFace(i, zp, getWorldCoord(zmC->pos))) {
            mesh.removeVertices(i);
            count++;
        }
        if(zpC && mesh.checkIsFace(i, zm, getWorldCoord(zpC->pos))) {
            mesh.removeVertices(i);
            count++;
        }

        if(count == 6)
            break;
    }

    if(chunk_xm && xmC_neighbor) {
        chunk_xm->removeFace(xp, xmC_neighbor->pos);
    }
    if(chunk_xp && xpC_neighbor) {
        chunk_xp->removeFace(xm, xpC_neighbor->pos);
    }
    if(chunk_ym && zmC_neighbor) {
        chunk_ym->removeFace(zp, zmC_neighbor->pos);
    }
    if(chunk_yp && zpC_neighbor) {
        chunk_yp->removeFace(zm, zpC_neighbor->pos);
    }
    
    updateBuffer();

    return true;
}



void Chunk::removeFace(Vertex vertex, glm::vec3 pos) {
    for(int i = 0; i < mesh.getVerticesSize(); i+=20) {
        if(mesh.checkIsFace(i, vertex, getWorldCoord(pos))) {
            mesh.removeVertices(i);
            updateBuffer();
            break;
        }
    }

}

#pragma endregion





void Chunk::updateMesh()
{
    mesh.clear();

    updateMesh(octree.getRoot());

    updateBuffer();
}


void Chunk::updateMesh(Node* node)
{
    if(node->childs[0]) {
        for(int i = 0; i < 8; i++) {
            updateMesh(node->childs[i]);
        }
        return;
    }

    Cube* chunk_xp_cube;
    Cube* chunk_xm_cube;
    Cube* chunk_yp_cube;
    Cube* chunk_ym_cube;


    Cube cube;

    for(int i = 0; i < node->cubes.size(); i++) {
        cube = node->cubes[i];

        chunk_xp_cube = nullptr;
        chunk_xm_cube = nullptr;
        chunk_yp_cube = nullptr;
        chunk_ym_cube = nullptr;


        if(cube.pos.x == CHUNK_WIDTH-1 && chunk_xp) {
            chunk_xp_cube = chunk_xp->get(glm::vec3(0, cube.pos.y, cube.pos.z));
        }
        if(cube.pos.x == 0 && chunk_xm) {
            chunk_xm_cube = chunk_xm->get(glm::vec3(CHUNK_WIDTH-1, cube.pos.y, cube.pos.z));
        }
        if(cube.pos.z == CHUNK_WIDTH-1 && chunk_yp) {
            chunk_yp_cube = chunk_yp->get(glm::vec3(cube.pos.x, cube.pos.y, 0));
        }
        if(cube.pos.z == 0 && chunk_ym) {
            chunk_ym_cube = chunk_ym->get(glm::vec3(cube.pos.x, cube.pos.y, CHUNK_WIDTH-1));
        }

        glm::vec3 cubeCoord = getWorldCoord(cube.pos);

        TextureCoordCube temp = TEXTURE_COORD.at(cube.id);

        if(!octree.get(glm::vec3(cube.pos.x + 1, cube.pos.y, cube.pos.z)) && !chunk_xp_cube) 
            mesh.addVertices(cubeCoord, xp, temp.side);
        
        if(!octree.get(glm::vec3(cube.pos.x - 1, cube.pos.y, cube.pos.z)) && !chunk_xm_cube) 
            mesh.addVertices(cubeCoord, xm, temp.side);
        
        if(!octree.get(glm::vec3(cube.pos.x, cube.pos.y + 1, cube.pos.z))) 
            mesh.addVertices(cubeCoord, yp, temp.top);
        
        if(!octree.get(glm::vec3(cube.pos.x, cube.pos.y - 1, cube.pos.z))) 
            mesh.addVertices(cubeCoord, ym, temp.bottom);
        
        if(!octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z + 1)) && !chunk_yp_cube) 
            mesh.addVertices(cubeCoord, zp, temp.side);
        
        if(!octree.get(glm::vec3(cube.pos.x, cube.pos.y, cube.pos.z - 1)) && !chunk_ym_cube) 
            mesh.addVertices(cubeCoord, zm, temp.side);

        
        if(chunk_xp_cube)
            chunk_xp->removeFace(xm, chunk_xp_cube->pos);
        if(chunk_xm_cube)
            chunk_xm->removeFace(xp, chunk_xm_cube->pos);
        if(chunk_yp_cube)
            chunk_yp->removeFace(zm, chunk_yp_cube->pos);
        if(chunk_ym_cube)
            chunk_ym->removeFace(zp, chunk_ym_cube->pos);

        
    }
}