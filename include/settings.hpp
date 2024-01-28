#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <string>
#include <fstream>

#include "nlohmann/json.hpp"

extern int CHUNK_WIDTH;
extern int CHUNK_HEIGHT;

bool loadChunkSettings(const std::string& path);


extern int RENDER_DISTANCE;

bool loadWorldSettings(const std::string& path);

#endif