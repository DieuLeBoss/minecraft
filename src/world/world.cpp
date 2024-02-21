#include "world/world.hpp"

using namespace nlohmann;

World::World() {}

World::World(glm::vec3* pos_player)
{
    terrain = ChunkManager(pos_player);
}

void World::update(glm::vec3 pos) {
    terrain.update(pos);
}

void World::draw() {
    terrain.draw();
}