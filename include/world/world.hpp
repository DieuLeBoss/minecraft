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
        std::vector<Chunk*> chunks;
        float renderDistance;

        void loadCubeJson();

        float calculateDist(glm::vec3 pos1, glm::vec3 pos2);

        glm::vec2 getGlobalPos(glm::vec2 local_pos);
        glm::vec3 getGlobalPosFromCamera(glm::vec2 local_pos, int pos_y_camera);

    public:
        World();
        
        Chunk* getChunk(glm::vec2 pos);
        void addChunk(glm::vec2 pos);
        void removeChunk(glm::vec2 pos);
        void removeChunk(int index);
        void draw();
        void test(glm::vec3 pos);
};

#endif