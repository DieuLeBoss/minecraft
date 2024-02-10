#ifndef TEXTURE_COORD_CUBE_HPP
#define TEXTURE_COORD_CUBE_HPP

#include "texture_coord_face.hpp"

struct TextureCoordCube
{
    TextureCoordCube() {}

    TextureCoordCube(
        TextureCoordFace ptop,
        TextureCoordFace pside,
        TextureCoordFace pbottom
    ): top(ptop), side(pside), bottom(pbottom) {}

    TextureCoordFace top;
    TextureCoordFace side;
    TextureCoordFace bottom;
};

#endif