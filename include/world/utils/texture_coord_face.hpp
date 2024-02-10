#ifndef TEXTURE_COORD_FACE_HPP
#define TEXTURE_COORD_FACE_HPP

struct TextureCoordFace
{
    TextureCoordFace() {}

    TextureCoordFace(
        float ax, float ay,
        float bx, float by,
        float cx, float cy,
        float dx, float dy
    ) {
        v[0][0] = ax;
        v[0][1] = ay;
        v[1][0] = bx;
        v[1][1] = by;
        v[2][0] = cx;
        v[2][1] = cy;
        v[3][0] = dx;
        v[3][1] = dy;
    }

    float v[4][2];
};

#endif