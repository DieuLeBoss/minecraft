#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include <GL/glew.h>

class VBO
{
    public:
        GLuint ID;
        VBO();
        VBO(GLfloat* vertices, GLsizeiptr size);

        void Bind();
        void Unbind();
        void Delete();

        void update(GLfloat* vertices, GLsizeiptr size);
};

#endif