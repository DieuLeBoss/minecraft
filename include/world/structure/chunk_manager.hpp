#ifndef CHUNK_MANAGER_HPP
#define CHUNK_MANAGER_HPP

#include <vector>
#include <string>
#include <thread>
#include <glm/glm.hpp>

#include "world/structure/chunk.hpp"

class ChunkManager
{
    private:
        std::vector<Chunk*> chunks;

        float calculateDist(glm::vec3 pos1, glm::vec3 pos2);

        glm::vec2 getGlobalPos(glm::vec2 local_pos);
        glm::vec3 getGlobalPosFromCamera(glm::vec2 local_pos, int pos_y_camera);
        
        Chunk* getChunk(glm::vec2 pos);
        void addChunk(glm::vec2 pos);
        void removeChunk(glm::vec2 pos);
        void removeChunk(int index);

    public:
        ChunkManager();
        ChunkManager(glm::vec3* pos_player);

        void generateDefault(glm::vec3* pos_player);
        void generateSuperFlat(glm::vec3* pos_player);

        void draw();
        void update(glm::vec3 pos);
};

#endif