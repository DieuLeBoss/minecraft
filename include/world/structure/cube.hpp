#ifndef CUBE_CLASS_HPP
#define CUBE_CLASS_HPP

#include <glm/glm.hpp>

struct Cube
{
    Cube()
    :pos(glm::vec3(0, 0, 0)), id(0) {}

    Cube(int pid)
    :pos(glm::vec3(0, 0, 0)), id(pid) {}

    Cube(glm::vec3 ppos, int pid)
    :pos(ppos), id(pid) {}

    Cube(Cube* cube)
    :pos(cube->pos), id(cube->id) {}

    bool isNull()
    {
        return id == -1;
    };

    glm::vec3 pos;
    int id;
};


#endif