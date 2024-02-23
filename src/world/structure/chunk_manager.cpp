#include "world/structure/chunk_manager.hpp"

ChunkManager::ChunkManager() {}

ChunkManager::ChunkManager(glm::vec3* pos_player) {
    chunks = std::vector<Chunk*>();

    procedural_generator = ProceduralGeneration();

    generateSuperFlat(pos_player);
}

void ChunkManager::addChunk(glm::vec2 pos)
{
    Chunk* chunk_xp = getChunk(glm::vec2(pos.x+1, pos.y));
    Chunk* chunk_xm = getChunk(glm::vec2(pos.x-1, pos.y));
    Chunk* chunk_yp = getChunk(glm::vec2(pos.x, pos.y+1));
    Chunk* chunk_ym = getChunk(glm::vec2(pos.x, pos.y-1));

    Chunk* chunk = new Chunk(pos, chunk_xp, chunk_xm, chunk_yp, chunk_ym);

    chunks.push_back(chunk);

    // for(int x = 0; x < CHUNK_WIDTH; x++) {
    //     for(int z = 0; z < CHUNK_WIDTH; z++) {
    //         chunk->addOctree(Cube(glm::vec3(x, 0, z), 8));
    //         for(int y = 1; y < 4; y++) {
    //             chunk->addOctree(Cube(glm::vec3(x, y, z), 1));
    //         }
    //         chunk->addOctree(Cube(glm::vec3(x, 4, z), 0));
    //     }
    // }

    procedural_generator.generateChunk(chunk);

    chunk->updateMesh();

    chunk->testBorder();  

    // a verifier si != nullptr connard de merde !!!!!!!

    if(chunk_xp) {
        chunk_xp->setChunkXm(chunk);
        chunk_xp->testBorder();
    }
    
    if(chunk_xm) {
        chunk_xm->setChunkXp(chunk);
        chunk_xm->testBorder();
    }
    
    if(chunk_yp) {
        chunk_yp->setChunkYm(chunk);
        chunk_yp->testBorder();
    }

    if(chunk_ym) {
        chunk_ym->setChunkYp(chunk);
        chunk_ym->testBorder();
    }
}

void ChunkManager::removeChunk(glm::vec2 pos) {
    Chunk* chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        if(chunk->getPosition() == pos) {
            chunk->unload();
            chunks.erase(chunks.begin()+i);
            delete(chunk);
        }
    }
}

void ChunkManager::removeChunk(int index) {
    Chunk* chunk = chunks.at(index);
    chunk->unload();
    chunks.erase(chunks.begin()+index);   
    delete(chunk);
}

Chunk* ChunkManager::getChunk(glm::vec2 pos) {
    Chunk* chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        if(chunk->getPosition() == pos) {
            return chunk;
        }
    }
    return nullptr;
}

glm::vec2 ChunkManager::getGlobalPos(glm::vec2 local_pos) {
    return glm::vec2(local_pos.x*CHUNK_WIDTH, local_pos.y*CHUNK_WIDTH);
}

glm::vec3 ChunkManager::getGlobalPosFromCamera(glm::vec2 local_pos, int pos_y_camera) {
    return glm::vec3(local_pos.x*CHUNK_WIDTH+CHUNK_WIDTH/2, pos_y_camera, local_pos.y*CHUNK_WIDTH+CHUNK_WIDTH/2);
}


void ChunkManager::draw()
{
    for(Chunk* c: chunks) {
        c->draw();
    }
}

void ChunkManager::updateRound(glm::vec3 pos) {
    Chunk* chunk;
    glm::vec2 local_pos;
    glm::vec3 global_pos;

    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        chunk->testBorder();

        if(chunk->isBorder()) {
            local_pos = chunk->getPosition();
            global_pos = getGlobalPosFromCamera(local_pos, pos.y);

            if(calculateDist(pos, global_pos) > RENDER_DISTANCE*CHUNK_WIDTH) {
                removeChunk(i);
            } else {
                if(!chunk->getChunkXp() && calculateDist(glm::vec3(global_pos.x+CHUNK_WIDTH, global_pos.y, global_pos.z), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x+1, local_pos.y));

                if(!chunk->getChunkXm() && calculateDist(glm::vec3(global_pos.x-CHUNK_WIDTH, global_pos.y, global_pos.z), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x-1, local_pos.y));

                if(!chunk->getChunkYp() && calculateDist(glm::vec3(global_pos.x, global_pos.y, global_pos.z+CHUNK_WIDTH), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x, local_pos.y+1));

                if(!chunk->getChunkYm() && calculateDist(glm::vec3(global_pos.x, global_pos.y, global_pos.z-CHUNK_WIDTH), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x, local_pos.y-1));

                chunk->testBorder();
            }
        }                
    }
}

void ChunkManager::updateSquare(glm::vec3 pos) {
    Chunk* chunk;
    glm::vec2 local_pos;
    glm::vec3 global_pos;

    glm::vec2 local_chunk_player_pos(static_cast<int>(pos.x/CHUNK_WIDTH), static_cast<int>(pos.z/CHUNK_WIDTH));

    int x_min = local_chunk_player_pos.x - RENDER_DISTANCE;
    int x_max = local_chunk_player_pos.x + RENDER_DISTANCE;
    int z_min = local_chunk_player_pos.y - RENDER_DISTANCE;
    int z_max = local_chunk_player_pos.y + RENDER_DISTANCE;



    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        chunk->testBorder();

        if(chunk->isBorder()) {
            local_pos = chunk->getPosition();
            global_pos = getGlobalPosFromCamera(local_pos, pos.y);

            if(!isIn(x_min, x_max, z_min, z_max, local_pos)) {
                removeChunk(i);
            } else {
                if(!chunk->getChunkXp() && isIn(x_min, x_max, z_min, z_max, glm::vec2(local_pos.x+1, local_pos.y)))
                    addChunk(glm::vec2(local_pos.x+1, local_pos.y));

                if(!chunk->getChunkXm() && isIn(x_min, x_max, z_min, z_max, glm::vec2(local_pos.x-1, local_pos.y)))
                    addChunk(glm::vec2(local_pos.x-1, local_pos.y));

                if(!chunk->getChunkYp() && isIn(x_min, x_max, z_min, z_max, glm::vec2(local_pos.x, local_pos.y+1)))
                    addChunk(glm::vec2(local_pos.x, local_pos.y+1));

                if(!chunk->getChunkYm() && isIn(x_min, x_max, z_min, z_max, glm::vec2(local_pos.x, local_pos.y-1)))
                    addChunk(glm::vec2(local_pos.x, local_pos.y-1));

                chunk->testBorder();
            }
        }                
    }
}

bool ChunkManager::isIn(int x_min, int x_max, int z_min, int z_max, glm::vec2 chunk_pos) {
    return (chunk_pos.x >= x_min && chunk_pos.x <= x_max && chunk_pos.y >= z_min && chunk_pos.y <= z_max);
}

float ChunkManager::calculateDist(glm::vec3 pos1, glm::vec3 pos2) {
    return sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2)+pow(pos1.z-pos2.z, 2));
}

void ChunkManager::generateSuperFlat(glm::vec3* pos_player) {
    addChunk(glm::vec2(int(pos_player->x/CHUNK_WIDTH), int(pos_player->z/CHUNK_WIDTH)));
}