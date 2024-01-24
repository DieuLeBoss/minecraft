#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>
#include <fstream>
#include <glm/glm.hpp>

#include "nlohmann/json.hpp"
#include "world/chunk.hpp"

#include "settings.hpp"

class World
{
    private:
        std::vector<TextureCoordCube> textureCoord;
        std::vector<Chunk> chunks;
        float renderDistance;

        void loadCubeJson();

        float calculateDist(glm::vec3 pos1, glm::vec3 pos2);

    public:
        World();
        
        Chunk* getChunk(glm::vec2 pos);
        void addChunk(glm::vec2 pos);
        void removeChunk(glm::vec2 pos);
        void draw();
        void test(glm::vec3 pos);
};

#endif