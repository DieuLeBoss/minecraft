#include "settings.hpp"

using namespace nlohmann;

#include <iostream>

int CHUNK_WIDTH = 0;
int CHUNK_HEIGHT = 0;
int CHUNK_SCALE = 0;

int RENDER_DISTANCE = 0;

std::vector<TextureCoordCube> TEXTURE_COORD = std::vector<TextureCoordCube>();


bool loadChunkSettings(const std::string& path) {
    std::ifstream file(path);

    if(!file.is_open())
        return false;

    json data;
    file >> data;

    file.close();

    auto dim = data["dimension"];

    CHUNK_WIDTH = dim["width"];
    CHUNK_HEIGHT = dim["height"];

    CHUNK_SCALE = data["scale"];

    return true;
}

bool loadWorldSettings(const std::string& path) {
    std::ifstream file(path);

    if(!file.is_open())
        return false;
    
    json data;
    file >> data;

    file.close();

    RENDER_DISTANCE = data["render distance"];

    return true;
}

bool loadTextureCoord(const std::string& path) {
    std::ifstream file(path);

    if(!file.is_open())
        return false;
    
    json data;
    file >> data;

    file.close();

    float size = 16.0f;
    float offset = 1/size;

    float top[2];
    float side[2];
    float bottom[2];

    int i = 0;

    for(const auto& e: data) {
        TextureCoordCube coord;

        i = 0;
        for(const auto& t: e["top"]) {
            top[i] = t;
            i++;
        }
        i = 0;
        for(const auto& t: e["side"]) {
            side[i] = t;
            i++;
        }
        i = 0;
        for(const auto& t: e["bottom"]) {
            bottom[i] = t;
            i++;
        }

        coord.top = TextureCoordFace(
            top[0]/size+offset, top[1]/size,
            top[0]/size+offset, top[1]/size + offset,
            top[0]/size       , top[1]/size + offset,
            top[0]/size       , top[1]/size
        );
        coord.side = TextureCoordFace(
            side[0]/size+offset, side[1]/size,
            side[0]/size+offset, side[1]/size + offset,
            side[0]/size       , side[1]/size + offset,
            side[0]/size       , side[1]/size
        );
        coord.bottom = TextureCoordFace(
            bottom[0]/size+offset, bottom[1]/size,
            bottom[0]/size+offset, bottom[1]/size + offset,
            bottom[0]/size       , bottom[1]/size + offset,
            bottom[0]/size       , bottom[1]/size
        );

        TEXTURE_COORD.push_back(coord);
    }

    return true;
}



bool loadSetting() {
    if(!loadChunkSettings("config/chunk.json")) {
        std::cerr << "Failed to load chunk settings !" << std::endl;
        return false;
    }
    if(!loadWorldSettings("config/world.json")) {
        std::cerr << "Failed to load world settings !" << std::endl;
        return false;
    }
    if(!loadTextureCoord("config/cube.json")) {
        std::cerr << "Failed to load texture coord settings !" << std::endl;
        return false;
    }
    return true;
}