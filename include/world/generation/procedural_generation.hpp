#ifndef PROCEDURAL_GENERATION_HPP
#define PROCEDURAL_GENERATION_HPP

#include <math.h>

#include "world/structure/chunk.hpp"
#include "world/generation/perlin_noise_2d.hpp"

#include "settings.hpp"

class ProceduralGeneration
{
    private:
        PerlinNoise2D perlin_noise;
    
    public:
        ProceduralGeneration();

        void generateChunk(Chunk* chunk);
};

#endif