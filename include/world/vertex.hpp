#ifndef VERTEX_HPP
#define VERTEX_HPP

struct Vertex
{
    Vertex()
    {
        v[0][0] = 0;
        v[0][1] = 0;
        v[0][2] = 0;
        v[1][0] = 0;
        v[1][1] = 0;
        v[1][2] = 0;
        v[2][0] = 0;
        v[2][1] = 0;
        v[2][2] = 0;
        v[3][0] = 0;
        v[3][1] = 0;
        v[3][2] = 0;
    }

    Vertex(float ax, float ay, float az,
           float bx, float by, float bz,
           float cx, float cy, float cz,
           float dx, float dy, float dz)
    {
        v[0][0] = ax;
        v[0][1] = ay;
        v[0][2] = az;
        v[1][0] = bx;
        v[1][1] = by;
        v[1][2] = bz;
        v[2][0] = cx;
        v[2][1] = cy;
        v[2][2] = cz;
        v[3][0] = dx;
        v[3][1] = dy;
        v[3][2] = dz;
    }

    float v[4][3];
};

//Vertex xp(1,0,0,1,1,0,1,1,1,1,0,1);
//Vertex xm(0,0,1,0,1,1,0,1,0,0,0,0);
//
//Vertex yp(0,1,0,0,1,1,1,1,1,1,1,0);
//Vertex ym(0,0,0,1,0,0,1,0,1,0,0,1);
//
//Vertex zp(1,0,1,1,1,1,0,1,1,0,0,1);
//Vertex zm(0,0,0,0,1,0,1,1,0,1,0,0);

#endif