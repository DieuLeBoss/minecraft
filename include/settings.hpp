#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <vector>
#include <string>
#include <fstream>

#include "nlohmann/json.hpp"

#include "world/utils/texture_coord_cube.hpp"

//--------------------------------------------- CHUNK SETTINGS ---------------------------------------------

extern int CHUNK_WIDTH;
extern int CHUNK_HEIGHT;
extern int CHUNK_SCALE;

bool loadChunkSettings(const std::string& path);


//--------------------------------------------- PLAYER SETTINGS ---------------------------------------------

extern int RENDER_DISTANCE;

bool loadPlayerSettings(const std::string& path);


//--------------------------------------------- WORLD SETTINGS ---------------------------------------------

extern int SEED;

bool loadWorldSettings(const std::string& path);


//--------------------------------------------- TEXTURE COORD SETTINGS ---------------------------------------------

extern std::vector<TextureCoordCube> TEXTURE_COORD;

bool loadTextureCoord(const std::string& path);


//--------------------------------------------- TEXTURE COORD SETTINGS ---------------------------------------------

extern int WATER_LEVEL;
extern int MIN_TERRAIN_LEVEL;

extern int PROCEDURAL_OCTAVES;
extern float PROCEDURAL_FREQUENCY;
extern float PROCEDURAL_AMPLITUDE;
extern float PROCEDURAL_PERSISTENCE;
extern float PROCEDURAL_MULT_FREQUENCY;

bool loadGenerationSetting(const std::string& path);


//--------------------------------------------- SETTINGS LOADER ---------------------------------------------

bool loadSetting();

#endif