#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>
#include <string>
#include <fstream>

#include "nlohmann/json.hpp"

#include "world/utils/texture_coord_cube.hpp"

extern int CHUNK_WIDTH;
extern int CHUNK_HEIGHT;
extern int CHUNK_SCALE;

bool loadChunkSettings(const std::string& path);


extern int RENDER_DISTANCE;

bool loadWorldSettings(const std::string& path);

extern std::vector<TextureCoordCube> TEXTURE_COORD;

bool loadTextureCoord(const std::string& path);

bool loadSetting();

#endif