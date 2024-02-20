#include "world/generation/perlin_noise_2d.hpp"

PerlinNoise2D::PerlinNoise2D() {
    std::mt19937 algo(SEED);

    int tmp[256];

    for(int v = 0; v < 256; v++) {
        tmp[v] = v;
    }

    std::shuffle(std::begin(tmp), std::end(tmp), algo);

    for(int i = 0; i < 256; i++) {
        permutation[i] = tmp[i];
        permutation[i+256] = tmp[i];
    }
}

float PerlinNoise2D::noise_2d(float x, float y) {
    // int X = std::floor(x) - std::floor(x/256) * 256;
    // int Y = std::floor(y) - std::floor(y/256) * 256;
    
    int X = static_cast<int>(std::floor(x)) & 255;
    int Y = static_cast<int>(std::floor(y)) & 255;

    float xf = x-floor(x);
    float yf = y-floor(y);

    glm::vec2 top_right(xf-1, yf-1);
    glm::vec2 top_left(xf, yf-1);
    glm::vec2 bottom_right(xf-1, yf);
    glm::vec2 bottom_left(xf, yf);

    int value_top_right = permutation[permutation[X+1]+Y+1];
    int value_top_left = permutation[permutation[X]+Y+1];
    int value_bottom_right = permutation[permutation[X+1]+Y];
    int value_bottom_left = permutation[permutation[X]+Y];

    float dot_top_right = glm::dot(top_right, getConstantVector(value_top_right));
    float dot_top_left = glm::dot(top_left, getConstantVector(value_top_left));
    float dot_bottom_right = glm::dot(bottom_right, getConstantVector(value_bottom_right));
    float dot_bottom_left = glm::dot(bottom_left, getConstantVector(value_bottom_left));

    float u = fade(xf);
    float v = fade(yf);

    return (lerp(u,
                lerp(v, dot_bottom_left, dot_top_left),
                lerp(v, dot_bottom_right, dot_top_right))+1)/2;
}

glm::vec2 PerlinNoise2D::getConstantVector(int v) {
    int h = v & 3;

    switch (h)
    {
        case 0:
            return glm::vec2(1, 1);
        case 1:
            return glm::vec2(-1, 1);
        case 2:
            return glm::vec2(-1, -1);
        default:
            return glm::vec2(1, -1);
    }
}

float PerlinNoise2D::fade(float t) {
    return ((6*t-15)*t+10)*t*t*t;
}

float PerlinNoise2D::lerp(float v, float a1, float a2) {
    return a1+v*(a2-a1);
}