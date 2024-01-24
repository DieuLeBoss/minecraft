#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "opengl/texture.hpp"

Texture::Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixelType)
{
    type = texType;

    glGenTextures(1, &ID);
    glActiveTexture(slot);
    glBindTexture(texType, ID);
    
    glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(image, &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(texType, 0, GL_RGBA, width, height, 0, format, pixelType, data);
        glGenerateMipmap(texType);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    glBindTexture(texType, 0);
}

void Texture::textUnit(Shader shader, const char* uniform, GLuint unit)
{
    GLuint text0Uni = glGetUniformLocation(shader.ID, uniform);
    shader.use();
    glUniform1i(text0Uni, unit);
}

void Texture::Bind()
{
    glBindTexture(type, ID);
}

void Texture::Unbind()
{
    glBindTexture(type, 0);
}

void Texture::Delete()
{
    glDeleteTextures(1, &ID);
}
