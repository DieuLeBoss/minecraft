#ifndef WORLD_HPP
#define WORLD_HPP

#include <thread>
#include <glm/glm.hpp>

#include "world/structure/chunk_manager.hpp"

#include "settings.hpp"

class World
{
    private:
        ChunkManager terrain;

    public:
        World(glm::vec3* pos_player);

        void generateTerrain();

        void update(glm::vec3 pos);
        void draw();
};

#endif