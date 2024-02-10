#include "world/structure/chunk_manager.hpp"

ChunkManager::ChunkManager() {
    chunks = std::vector<Chunk>();
}

void ChunkManager::addChunk(glm::vec2 pos)
{
    Chunk* chunk_xp = getChunk(glm::vec2(pos.x+1, pos.y));
    Chunk* chunk_xm = getChunk(glm::vec2(pos.x-1, pos.y));
    Chunk* chunk_yp = getChunk(glm::vec2(pos.x, pos.y+1));
    Chunk* chunk_ym = getChunk(glm::vec2(pos.x, pos.y-1));

    Chunk chunk(pos, chunk_xp, chunk_xm, chunk_yp, chunk_ym);

    chunks.push_back(chunk);

    for(int x = 0; x < CHUNK_WIDTH; x++) {
        for(int z = 0; z < CHUNK_WIDTH; z++) {
            chunk.addOctree(Cube(glm::vec3(x, 0, z), 8));
            for(int y = 1; y < 4; y++) {
                chunk.addOctree(Cube(glm::vec3(x, y, z), 1));
            }
            chunk.addOctree(Cube(glm::vec3(x, 4, z), 0));
        }
    }

    chunk.updateMesh();

    chunk.testBorder();  

    // a verifier si != nullptr connard de merde !!!!!!!

    if(chunk_xp) {
        chunk_xp->setChunkXm(&chunk);
        chunk_xp->testBorder();
    }
    
    if(chunk_xm) {
        chunk_xm->setChunkXp(&chunk);
        chunk_xm->testBorder();
    }
    
    if(chunk_yp) {
        chunk_yp->setChunkYm(&chunk);
        chunk_yp->testBorder();
    }

    if(chunk_ym) {
        chunk_ym->setChunkYp(&chunk);
        chunk_ym->testBorder();
    }
}

void ChunkManager::removeChunk(glm::vec2 pos) {
    Chunk chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        if(chunk.getPosition() == pos) {
            chunk.unload();
            chunks.erase(chunks.begin()+i);
        }
    }
}

void ChunkManager::removeChunk(int index) {
    Chunk chunk = chunks.at(index);
    chunk.unload();
    chunks.erase(chunks.begin()+index);   
}

Chunk* ChunkManager::getChunk(glm::vec2 pos) {
    Chunk chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        if(chunk.getPosition() == pos) {
            return &chunk;
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
    for(Chunk c: chunks) {
        c.draw();
    }
}

void ChunkManager::update(glm::vec3 pos) {
    Chunk chunk;
    glm::vec2 local_pos;
    glm::vec3 global_pos;

    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        chunk.testBorder();

        if(chunk.isBorder()) {
            local_pos = chunk.getPosition();
            global_pos = getGlobalPosFromCamera(local_pos, pos.y);

            if(calculateDist(pos, global_pos) > RENDER_DISTANCE*CHUNK_WIDTH) {
                removeChunk(i);
            } else {
                if(!chunk.getChunkXp() && calculateDist(glm::vec3(global_pos.x+CHUNK_WIDTH, global_pos.y, global_pos.z), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x+1, local_pos.y));

                if(!chunk.getChunkXm() && calculateDist(glm::vec3(global_pos.x-CHUNK_WIDTH, global_pos.y, global_pos.z), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x-1, local_pos.y));

                if(!chunk.getChunkYp() && calculateDist(glm::vec3(global_pos.x, global_pos.y, global_pos.z+CHUNK_WIDTH), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x, local_pos.y+1));

                if(!chunk.getChunkYm() && calculateDist(glm::vec3(global_pos.x, global_pos.y, global_pos.z-CHUNK_WIDTH), pos) < RENDER_DISTANCE*(CHUNK_WIDTH-1))
                    addChunk(glm::vec2(local_pos.x, local_pos.y-1));

                chunk.testBorder();
            }
        }                
    }
}

float ChunkManager::calculateDist(glm::vec3 pos1, glm::vec3 pos2) {
    return sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2)+pow(pos1.z-pos2.z, 2));
}