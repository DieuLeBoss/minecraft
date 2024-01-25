#include "world/world.hpp"

using namespace nlohmann;

World::World()
{
    chunks = std::vector<Chunk>();
    textureCoord = std::vector<TextureCoordCube>();
    renderDistance = 10;

    loadCubeJson();
}

void World::loadCubeJson()
{
    std::ifstream file("config/cube.json");
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

        textureCoord.push_back(coord);
    }
}

void World::addChunk(glm::vec2 pos)
{
    Chunk* chunk_xp = getChunk(glm::vec2(pos.x+1, pos.y));
    Chunk* chunk_xm = getChunk(glm::vec2(pos.x-1, pos.y));
    Chunk* chunk_yp = getChunk(glm::vec2(pos.x, pos.y+1));
    Chunk* chunk_ym = getChunk(glm::vec2(pos.x, pos.y-1));

    Chunk temp(pos, &textureCoord, chunk_xp, chunk_xm, chunk_yp, chunk_ym);

    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            for(int y = 0; y < 10; y++) {
                temp.add(Cube(glm::vec3(x, y, z), 2));
            }
        }
    }

    if(getChunk(glm::vec2(pos.x+1, pos.y)) != nullptr && getChunk(glm::vec2(pos.x-1, pos.y)) != nullptr && getChunk(glm::vec2(pos.x, pos.y+1)) != nullptr && getChunk(glm::vec2(pos.x, pos.y-1)) != nullptr) {
        temp.unsetBorder();
    } else {
        temp.setBorder();
    }

    chunks.push_back(temp);

    Chunk* chunk = &chunks.at(chunks.size()-1);
    

    // a verifier si != nullptr connard de merde !!!!!!!

    if(chunk_xp != nullptr) {
        chunk_xp->setChunkXm(chunk);
    }
    if(chunk_xm != nullptr) {
        chunk_xm->setChunkXp(chunk);
    }
    if(chunk_yp != nullptr) {
        chunk_yp->setChunkYm(chunk);
    }
    if(chunk_ym != nullptr) {
        chunk_ym->setChunkYp(chunk);
    }
}

Chunk* World::getChunk(glm::vec2 pos) {
    Chunk* chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = &chunks.at(i);
        if(chunk->getPosition() == pos) {
            return chunk;
        }
    }
    return nullptr;
}

void World::draw()
{
    for(Chunk c: chunks) {
        c.draw();
    }
}

void World::test(glm::vec3 pos) {
    Chunk* chunk;
    glm::vec2 temp;
    glm::vec3 chunkPos;
    glm::vec3 chunkSidePos;
    float dist;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = &chunks.at(i);
        temp = chunk->getPosition();
        chunkPos = glm::vec3(temp.x*CHUNK_X_SIZE+CHUNK_X_SIZE/2, pos.y, temp.y*CHUNK_Z_SIZE+CHUNK_Z_SIZE/2);
        dist = calculateDist(pos, chunkPos);

        if(dist > renderDistance*16) {
            chunks.erase(chunks.begin()+i);
        } else if(chunk->isBorder()) {
            chunkSidePos = glm::vec3(chunkPos.x+1, pos.y, chunkPos.z);
            if(getChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z)) == nullptr && calculateDist(pos, chunkSidePos) < renderDistance*16) {
                addChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z));
            }
            chunkSidePos = glm::vec3(chunkPos.x-1, pos.y, chunkPos.z);
            if(getChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z)) == nullptr && calculateDist(pos, chunkSidePos) < renderDistance*16) {
                addChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z));
            }
            chunkSidePos = glm::vec3(chunkPos.x, pos.y, chunkPos.z+1);
            if(getChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z)) == nullptr && calculateDist(pos, chunkSidePos) < renderDistance*16) {
                addChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z));
            }
            chunkSidePos = glm::vec3(chunkPos.x, pos.y, chunkPos.z-1);
            if(getChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z)) == nullptr && calculateDist(pos, chunkSidePos) < renderDistance*16) {
                addChunk(glm::vec2(chunkSidePos.x, chunkSidePos.z));
            }
        }
    }
}

float World::calculateDist(glm::vec3 pos1, glm::vec3 pos2) {
    return sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2)+pow(pos1.z-pos2.z, 2));
}