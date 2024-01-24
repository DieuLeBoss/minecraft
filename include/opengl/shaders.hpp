#ifndef SHADERS_H
#define SHADERS_H

#include <GL/glew.h>
#include <vector>
#include <fstream>
#include <sstream>

class Shader
{
    public:
        Shader(const char* vPath, const char* fPath);

        void setInt(const std::string &name, float value);
        void use();
        void kill();

        GLuint ID;

};



#endif