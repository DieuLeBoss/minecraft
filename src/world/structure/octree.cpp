#include "world/structure/octree.hpp"

using namespace std;

Octree::Octree()
{
    root = Node(Zone(0, 0, 0, CHUNK_WIDTH, CHUNK_HEIGHT, CHUNK_WIDTH)); // config octree
    MAX_SIZE_LIST = 1024; // config octree
    cubes = std::vector<Cube>();
}

bool Octree::intersect(Zone zone, glm::vec3 pos)
{
    if(pos.x >= zone.minX && pos.x < zone.maxX &&
       pos.y >= zone.minY && pos.y < zone.maxY &&
       pos.z >= zone.minZ && pos.z < zone.maxZ) {
        return true;
    }

    return false;
}

void Octree::divNode(Node* node)
{
    Zone* zone = &node->zone;

    int midX = zone->minX + (zone->maxX - zone->minX)/2;
    int midY = zone->minY + (zone->maxY - zone->minY)/2;
    int midZ = zone->minZ + (zone->maxZ - zone->minZ)/2;
    
    node->childs[0] = new Node(Zone(zone->minX, zone->minY, zone->minZ, midX, midY, midZ));
    node->childs[1] = new Node(Zone(midX, zone->minY, zone->minZ, zone->maxX, midY, midZ));
    node->childs[2] = new Node(Zone(zone->minX, zone->minY, midZ,midX, midY, zone->maxZ));
    node->childs[3] = new Node(Zone(midX, zone->minY, midZ,zone->maxX, midY, zone->maxZ));
    node->childs[4] = new Node(Zone(zone->minX, midY, zone->minZ,midX, zone->maxY, midZ));
    node->childs[5] = new Node(Zone(midX, midY, zone->minZ, zone->maxX, zone->maxY, midZ));
    node->childs[6] = new Node(Zone(zone->minX, midY, midZ,midX, zone->maxY, zone->maxZ));
    node->childs[7] = new Node(Zone(midX, midY, midZ,zone->maxX, zone->maxY, zone->maxZ));

}

bool Octree::add(Cube cube)
{
    if(get(cube.pos))
        return false;
    return addRecursive(&root, cube);
}

bool Octree::addRecursive(Node* node, Cube cube)
{
    if(node->childs[0] != nullptr) {
        Node* child;

        for(int i = 0; i < 8; i++) {
            child = node->childs[i];

            if(intersect(child->zone, cube.pos)) {
                return addRecursive(node->childs[i], cube);
            }
        }
        return false;
    }

    node->cubes.push_back(cube);

    if(node->cubes.size() >= (long unsigned int)MAX_SIZE_LIST) {
        divNode(node);

        Cube cubeTemp;
        Node* child;

        for(int cubeIndex = 0; cubeIndex < node->cubes.size(); cubeIndex++) {
            cubeTemp = node->cubes.at(cubeIndex);
            
            for(int nodeIndex = 0; nodeIndex < 8; nodeIndex++) {
                child = node->childs[nodeIndex];

                if(intersect(child->zone, cubeTemp.pos)) {
                    child->cubes.push_back(cubeTemp);
                    break;
                }
            }
        }

        node->cubes.clear();
    }

    return true;
}

Cube* Octree::get(glm::vec3 pos)
{
    if(!intersect(root.zone, pos)) {
        return nullptr;
    }

    return getR(&root, pos);
}

Cube* Octree::getR(Node* node, glm::vec3 pos)
{
    if(node->childs[0] != nullptr) {
        Node* child;

        for(int i = 0; i < 8; i++) {
            child = node->childs[i];

            if(intersect(child->zone, pos)) {
                return getR(child, pos);
            }
        }
    }

    Cube* cube;

    for(int i = 0; i < node->cubes.size(); i++) {
        cube = &node->cubes.at(i);
        if(cube->pos.x == pos.x && cube->pos.y == pos.y && cube->pos.z == pos.z) {
            return cube;
        }
    }
    return nullptr;
}

Cube Octree::remove(glm::vec3 pos)
{
    return removeR(&root, pos);
}

Cube Octree::removeR(Node* node, glm::vec3 pos)
{
    if(node->childs[0] != nullptr) {
        Node* child;

        for(int i = 0; i < 8; i++) {
            child = node->childs[i];

            if(this->intersect(child->zone, pos)) {
                return this->removeR(child, pos);
            }
        }
    }

    Cube* cube;

    for(int i = 0; i < node->cubes.size(); i++) {
        cube = &node->cubes.at(i);
        if(cube->pos.x == pos.x && cube->pos.y == pos.y && cube->pos.z == pos.z) {
            Cube output = Cube(cube);
            node->cubes.erase(node->cubes.begin()+i);
            return output;
        }
    }

    return Cube(-1);
}

Node* Octree::getRoot()
{
    return &root;
}