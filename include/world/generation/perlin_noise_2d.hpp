#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include <iostream>

#include <cmath>
#include <random>
#include <algorithm>
#include <glm/glm.hpp>

#include "settings.hpp"

class PerlinNoise2D
{
    private:
        int permutation[256*2];

        glm::vec2 getConstantVector(int v);

        float fade(float t);
        float lerp(float v, float a1, float a2);

    public:
        PerlinNoise2D();

        float noise_2d(float x, float y);
};

#endif