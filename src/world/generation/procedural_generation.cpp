#include "world/generation/procedural_generation.hpp"

ProceduralGeneration::ProceduralGeneration() {
    perlin_noise = PerlinNoise2D();
}

void ProceduralGeneration::generateChunk(Chunk* chunk) {
    glm::vec2 pos = chunk->getPosition();
    float value;
    int height;

    for(int x = 0; x < CHUNK_WIDTH; x++) {
        for(int z = 0; z < CHUNK_WIDTH; z++) {
            height = 0;
            
            for(int i = 0; i < PROCEDURAL_OCTAVES; i++) {
                value = perlin_noise.noise_2d((pos.x*CHUNK_WIDTH+x)*PROCEDURAL_FREQUENCY*std::pow(PROCEDURAL_MULT_FREQUENCY, i),
                                            (pos.y*CHUNK_WIDTH+z)*PROCEDURAL_FREQUENCY*std::pow(PROCEDURAL_MULT_FREQUENCY, i));
                height += static_cast<int>(value*PROCEDURAL_AMPLITUDE*std::pow(PROCEDURAL_PERSISTENCE, i));
            }

            // chunk->addOctree(Cube(glm::vec3(x, 0, z), 8));

            // for(int y = 1; y < height; y++) {
            //     chunk->addOctree(Cube(glm::vec3(x, y, z), 1));
            // }

            chunk->addOctree(Cube(glm::vec3(x, MIN_TERRAIN_LEVEL+height-1, z), 1));
            chunk->addOctree(Cube(glm::vec3(x, MIN_TERRAIN_LEVEL+height, z), 0));
        }
    }
}