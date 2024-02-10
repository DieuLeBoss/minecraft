#ifndef OCTREE_CLASS_HPP
#define OCTREE_CLASS_HPP

#include <iostream>
#include <vector>

#include "world/structure/cube.hpp"

#include "settings.hpp"

struct Zone
{
    Zone()
    :minX(0), minY(0), minZ(0),
     maxX(0), maxY(0), maxZ(0) {};

    Zone(float pminX, float pminY, float pminZ, 
         float pmaxX, float pmaxY, float pmaxZ)
    :minX(pminX), minY(pminY), minZ(pminZ),
     maxX(pmaxX), maxY(pmaxY), maxZ(pmaxZ) {};

    float minX, minY, minZ;
    float maxX, maxY, maxZ;
};

struct Node
{
    Node() 
    :zone(Zone()) {
        for (int i = 0; i < 8; ++i) {
            childs[i] = nullptr;
        }
        cubes = std::vector<Cube>();
    }
    
    Node(Zone pzone)
    :zone(pzone) {
        for (int i = 0; i < 8; ++i) {
            childs[i] = nullptr;
        }
        cubes = std::vector<Cube>();
    };

    Zone zone;
    Node* childs[8];
    std::vector<Cube> cubes;
    bool end;
};

class Octree
{
    private:
        Node root;
        int MAX_SIZE_LIST;
        std::vector<Cube> cubes;

        bool addRecursive(Node* node, Cube cube);
        Cube removeR(Node* node, glm::vec3 pos);
        Cube* getR(Node* node, glm::vec3 pos);

        bool intersect(Zone zone, glm::vec3 pos);
        void divNode(Node* node);

    public:
        Octree();

        bool add(Cube cube);
        Cube remove(glm::vec3 pos);
        Cube* get(glm::vec3 pos);
        Node* getRoot();
};

#endif