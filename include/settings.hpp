#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <fstream>

#include "nlohmann/json.hpp"

extern int CHUNK_X_SIZE;
extern int CHUNK_Y_SIZE;
extern int CHUNK_Z_SIZE;

bool loadChunkSettings(const std::string& path);


extern int RENDER_DISTANCE;

bool loadWorldSettings(const std::string& path);

#endif