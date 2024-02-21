#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <GL/glew.h>

class EBO
{
    public:
        GLuint ID;
        EBO();
        EBO(GLuint* indices, GLsizeiptr size);

        void generate();
        void Bind();
        void Unbind();
        void Delete();

        void update(GLuint* indices, GLsizeiptr size);
};

#endif