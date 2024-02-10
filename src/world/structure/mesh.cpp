#include "world/structure/mesh.hpp"

Mesh::Mesh() {

}


void Mesh::addVertices(glm::vec3 pos, Vertex v, TextureCoordFace t) {
    vertices.insert(vertices.end(), {
        pos.x + v.v[0][0]/CHUNK_SCALE, pos.y + v.v[0][1]/CHUNK_SCALE, pos.z + v.v[0][2]/CHUNK_SCALE, t.v[0][0], t.v[0][1],
        pos.x + v.v[1][0]/CHUNK_SCALE, pos.y + v.v[1][1]/CHUNK_SCALE, pos.z + v.v[1][2]/CHUNK_SCALE, t.v[1][0], t.v[1][1],
        pos.x + v.v[2][0]/CHUNK_SCALE, pos.y + v.v[2][1]/CHUNK_SCALE, pos.z + v.v[2][2]/CHUNK_SCALE, t.v[2][0], t.v[2][1],
        pos.x + v.v[3][0]/CHUNK_SCALE, pos.y + v.v[3][1]/CHUNK_SCALE, pos.z + v.v[3][2]/CHUNK_SCALE, t.v[3][0], t.v[3][1]
    });

    GLuint size = static_cast<GLuint>(vertices.size()-20)/5;

    indices.push_back(size+0);
    indices.push_back(size+1);
    indices.push_back(size+2);
    indices.push_back(size+0);
    indices.push_back(size+2);
    indices.push_back(size+3);
}


void Mesh::removeVertices(int i) {
    int j = (i / 20) * 6;

    vertices.erase(vertices.begin()+i, vertices.begin()+i+20);
    indices.erase(indices.begin()+j, indices.begin()+j+6);
    
    if(indices.size() > 0)
        updateIndices(j);
}


bool Mesh::hasFace(Vertex v, glm::vec3 pos) {
    for(int i = 0; i < vertices.size(); i+=20) {
        if(checkIsFace(i, v, pos))
            return true;
    }
    return false;
}


bool Mesh::checkIsFace(int i, Vertex v, glm::vec3 pos) {
    if(i >= vertices.size())
        return false;
    return (pos.x + v.v[0][0]/CHUNK_SCALE == vertices.at(i+0)) && (pos.y + v.v[0][1]/CHUNK_SCALE == vertices.at(i+1)) && (pos.z + v.v[0][2]/CHUNK_SCALE == vertices.at(i+2)) &&
           (pos.x + v.v[1][0]/CHUNK_SCALE == vertices.at(i+5)) && (pos.y + v.v[1][1]/CHUNK_SCALE == vertices.at(i+6)) && (pos.z + v.v[1][2]/CHUNK_SCALE == vertices.at(i+7)) &&
           (pos.x + v.v[2][0]/CHUNK_SCALE == vertices.at(i+10)) && (pos.y + v.v[2][1]/CHUNK_SCALE == vertices.at(i+11)) && (pos.z + v.v[2][2]/CHUNK_SCALE == vertices.at(i+12)) &&
           (pos.x + v.v[3][0]/CHUNK_SCALE == vertices.at(i+15)) && (pos.y + v.v[3][1]/CHUNK_SCALE == vertices.at(i+16)) && (pos.z + v.v[3][2]/CHUNK_SCALE == vertices.at(i+17));
}


void Mesh::updateIndices(int i) {
    for(int k = i; k < indices.size(); k++) {
        indices.at(k) = indices.at(k)-4;
    }
}


void Mesh::clear() {
    vertices.clear();
    indices.clear();
}


GLfloat* Mesh::getVerticesData() {
    return vertices.data();
}


GLuint* Mesh::getIndicesData() {
    return indices.data();
}


unsigned long Mesh::getVerticesSize() {
    return vertices.size();
}


unsigned long Mesh::getIndicesSize() {
    return indices.size();
}

