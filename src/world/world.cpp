#include "world/world.hpp"

using namespace nlohmann;

World::World()
{
    chunks = std::vector<Chunk*>();
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

    Chunk* temp = new Chunk(pos, &textureCoord, chunk_xp, chunk_xm, chunk_yp, chunk_ym);

    chunks.push_back(temp);

    for(int x = 0; x < 16; x++) {
        for(int z = 0; z < 16; z++) {
            temp->add(Cube(glm::vec3(x, 0, z), 8));
            for(int y = 1; y < 4; y++) {
                temp->add(Cube(glm::vec3(x, y, z), 1));
            }
            temp->add(Cube(glm::vec3(x, 4, z), 0));
        }
    }

    temp->testBorder();  

    // a verifier si != nullptr connard de merde !!!!!!!

    if(chunk_xp) {
        chunk_xp->setChunkXm(temp);
        chunk_xp->testBorder();
    }
    
    if(chunk_xm) {
        chunk_xm->setChunkXp(temp);
        chunk_xm->testBorder();
    }
    
    if(chunk_yp) {
        chunk_yp->setChunkYm(temp);
        chunk_yp->testBorder();
    }

    if(chunk_ym) {
        chunk_ym->setChunkYp(temp);
        chunk_ym->testBorder();
    }
}

void World::removeChunk(glm::vec2 pos) {
    Chunk* chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        if(chunk->getPosition() == pos) {
            chunk->unload();
            chunks.erase(chunks.begin()+i);
        }
    }
}

void World::removeChunk(int index) {
    chunks.at(index)->unload();
    chunks.erase(chunks.begin()+index);   
}

Chunk* World::getChunk(glm::vec2 pos) {
    Chunk* chunk;
    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        if(chunk->getPosition() == pos) {
            return chunk;
        }
    }
    return nullptr;
}

glm::vec2 World::getGlobalPos(glm::vec2 local_pos) {
    return glm::vec2(local_pos.x*CHUNK_WIDTH, local_pos.y*CHUNK_WIDTH);
}

glm::vec3 World::getGlobalPosFromCamera(glm::vec2 local_pos, int pos_y_camera) {
    return glm::vec3(local_pos.x*CHUNK_WIDTH+CHUNK_WIDTH/2, pos_y_camera, local_pos.y*CHUNK_WIDTH+CHUNK_WIDTH/2);
}


void World::draw()
{
    for(Chunk* c: chunks) {
        c->draw();
    }
}

void World::test(glm::vec3 pos) {
    Chunk* chunk;
    glm::vec2 local_pos;
    glm::vec3 global_pos;

    for(int i = 0; i < chunks.size(); i++) {
        chunk = chunks.at(i);
        local_pos = chunk->getPosition();
        global_pos = getGlobalPosFromCamera(local_pos, pos.y);

        if(calculateDist(pos, global_pos) > renderDistance*CHUNK_WIDTH) {
            removeChunk(i);
        } else if(chunk->isBorder()) {
            if(!chunk->getChunkXp() && calculateDist(glm::vec3(global_pos.x+CHUNK_WIDTH, global_pos.y, global_pos.z), pos) < renderDistance*(CHUNK_WIDTH-1))
                addChunk(glm::vec2(local_pos.x+1, local_pos.y));

            if(!chunk->getChunkXm() && calculateDist(glm::vec3(global_pos.x-CHUNK_WIDTH, global_pos.y, global_pos.z), pos) < renderDistance*(CHUNK_WIDTH-1))
                addChunk(glm::vec2(local_pos.x-1, local_pos.y));

            if(!chunk->getChunkYp() && calculateDist(glm::vec3(global_pos.x, global_pos.y, global_pos.z+CHUNK_WIDTH), pos) < renderDistance*(CHUNK_WIDTH-1))
                addChunk(glm::vec2(local_pos.x, local_pos.y+1));

            if(!chunk->getChunkYm() && calculateDist(glm::vec3(global_pos.x, global_pos.y, global_pos.z-CHUNK_WIDTH), pos) < renderDistance*(CHUNK_WIDTH-1))
                addChunk(glm::vec2(local_pos.x, local_pos.y-1));
        }

        chunk->testBorder();        
    }
}

float World::calculateDist(glm::vec3 pos1, glm::vec3 pos2) {
    return sqrt(pow(pos1.x-pos2.x, 2)+pow(pos1.y-pos2.y, 2)+pow(pos1.z-pos2.z, 2));
}