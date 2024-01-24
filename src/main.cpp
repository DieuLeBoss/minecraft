#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "opengl/shaders.hpp"
#include "opengl/texture.hpp"
#include "camera.hpp"

#include "world/chunk.hpp"
#include "world/world.hpp"

#include "settings.hpp"

#include "nlohmann/json.hpp"

bool checkChunk(glm::mat4 matrix, glm::vec3 pos) {
    glm::vec4 clipSpace = matrix * glm::vec4(pos, 1);

    return (clipSpace.x >= -1 && clipSpace.x <= 1 &&
            clipSpace.y >= -1 && clipSpace.y <= 1 &&
            clipSpace.z >= -1 && clipSpace.z <= 1);
}

int minecraft()
{
    #pragma region "Initialization and creation of the context and the window"

    if (!glfwInit())
    {
        std::cerr << "Failed to init GLFW" << std::endl;
        return -1;
    }

    // Dire à GLFW quelle version d'openGL utiliser, ici on utilise openGL 3.3 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Dire à GLFW qu'on utlise le CORE_PROFILE, donc que les fonctions modernes
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWmonitor* primMonitor = glfwGetPrimaryMonitor();

    const GLFWvidmode* mode = glfwGetVideoMode(primMonitor);

    int WIDTH = mode->width;
    int HEIGHT = mode->height;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft", primMonitor, NULL);

    /*int WIDTH = 1920;
    int HEIGHT = 1080;

    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft Project", NULL, NULL);*/

    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GLEW" << std::endl;
        glfwDestroyWindow(window); 
        glfwTerminate();
        return -1;
    }

    // Specify the viewport of openGL in the window
    glViewport(0, 0, WIDTH, HEIGHT);

    #pragma endregion

    if(!loadChunkSettings("config/chunk.json")) {
        std::cerr << "Failed to load chunk settings !" << std::endl;
        return -1;
    }
    if(!loadWorldSettings("config/world.json")) {
        std::cerr << "Failed to load world settings !" << std::endl;
        return -1;
    }

    #pragma region "Importation of shaders and textures, and creation of the world"

    Shader shader("res/shaders/defaultV.glsl", "res/shaders/defaultF.glsl");

    World world = World();

    for(int x = 0; x < 2; x++) {
        for(int z = 0; z < 2; z++) {
            world.addChunk(glm::vec2(x, z));
        }
    }

    Texture tex("res/textures/terrain.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex.textUnit(shader, "tex0", 0);

    glEnable(GL_DEPTH_TEST); // permet de ne pas afficher les objets cacher par d'autre 
    glEnable(GL_CULL_FACE); // permet d'activer le culling (affiche les triangles que dans une certaine orientation)

    #pragma endregion

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), window);

    
    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.917f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 10000.0f);

        shader.use();

        camera.Matrix(shader, "camMatrix");

        tex.Bind();
        
        //world.test(camera.getPosition());

        world.draw();

        //if(checkChunk(camera.getCameraMatrix(), chunk.getPosition()))
        
        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    shader.kill();

    tex.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

int main()
{
    minecraft();

    // verifer dernier implementation dans world, puis dans chunk dans add, probleme core dump a intersect quand get dans chunk voisin
}