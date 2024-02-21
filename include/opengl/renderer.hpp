#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "opengl/shaders.hpp"
#include "opengl/texture.hpp"

#include "world/world.hpp"

#include "camera.hpp"

class Renderer
{
    private:
        GLFWwindow* window;

        Shader shader;
        Texture texture;

        int WIDTH;
        int HEIGHT;

    public:
        Renderer();
        int init(const char* title_window); // peut-etre utiliser un dico pour stocker les shaders et textures en fonction de leurs nom
        void render(Camera& camera, World& world);

        void terminate();

        GLFWwindow* getWindow();
        int getWidth();
        int getHeight();
};


#endif