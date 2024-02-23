#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include <vector>
#include <string>
#include <thread>
#include <glm/glm.hpp>

#include "camera.hpp"
#include "world/structure/chunk.hpp"
#include "world/generation/procedural_generation.hpp"

class ChunkManager
{
    private:
        std::vector<Chunk*> chunks;

        ProceduralGeneration procedural_generator;

        float calculateDist(glm::vec3 pos1, glm::vec3 pos2);

        glm::vec2 getGlobalPos(glm::vec2 local_pos);
        glm::vec3 getGlobalPosFromCamera(glm::vec2 local_pos, int pos_y_camera);
        
        Chunk* getChunk(glm::vec2 pos);
        void addChunk(glm::vec2 pos);
        void removeChunk(glm::vec2 pos);
        void removeChunk(int index);

        bool isIn(int x_min, int x_max, int z_min, int z_max, glm::vec2 chunk_pos);

    public:
        ChunkManager();
        ChunkManager(glm::vec3* pos_player);

        void generateDefault(glm::vec3* pos_player);
        void generateSuperFlat(glm::vec3* pos_player);

        void draw();
        void updateRound(glm::vec3 pos);
        void updateSquare(glm::vec3 pos);
};

#endif