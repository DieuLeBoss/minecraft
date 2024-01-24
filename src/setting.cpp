#include "settings.hpp"

using namespace nlohmann;

#include <iostream>

int CHUNK_X_SIZE = 0;
int CHUNK_Y_SIZE = 0;
int CHUNK_Z_SIZE = 0;

int RENDER_DISTANCE = 0;





bool loadChunkSettings(const std::string& path) {
    std::ifstream file(path);

    if(!file.is_open())
        return false;

    json data;
    file >> data;

    file.close();

    auto dim = data["dimension"];

    CHUNK_X_SIZE = dim["X"];
    CHUNK_Y_SIZE = dim["Y"];
    CHUNK_Z_SIZE = dim["Z"];

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