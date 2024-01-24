#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <GL/glew.h>

#include "shaders.hpp"



class Texture
{
    public:
        GLuint ID;
        GLenum type; 
        int NB;
        Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType);

        void textUnit(Shader shader, const char* uniform, GLuint unit);
        void Bind();
        void Unbind();
        void Delete();
};

#endif