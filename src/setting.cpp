#include "settings.hpp"

using namespace nlohmann;

#include <iostream>

int CHUNK_WIDTH = 0;
int CHUNK_HEIGHT = 0;

int RENDER_DISTANCE = 0;


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