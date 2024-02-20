#include <iostream>
#include <vector>
#include <thread>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "opengl/shaders.hpp"
#include "opengl/texture.hpp"
#include "camera.hpp"

#include "world/structure/chunk.hpp"
#include "world/world.hpp"

#include "settings.hpp"

#include "nlohmann/json.hpp"

#include <cmath>
#include "world/generation/perlin_noise_2d.hpp"

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

    if(!loadSetting())
        return -1;

    #pragma region "Importation of shaders and textures, and creation of the world"

    Shader shader("res/shaders/defaultV.glsl", "res/shaders/defaultF.glsl");

    Texture tex("res/textures/terrain.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex.textUnit(shader, "tex0", 0);

    glEnable(GL_DEPTH_TEST); // permet de ne pas afficher les objets cacher par d'autre 
    glEnable(GL_CULL_FACE); // permet d'activer le culling (affiche les triangles que dans une certaine orientation)

    #pragma endregion


    glm::vec3 player_pos(0.0f, 256.0f, 0.0f);

    Camera camera(WIDTH, HEIGHT, player_pos, window);

    World world = World(camera.getPointerPosition());


    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.917f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(90.0f, 0.1f, 10000.0f);

        shader.use();

        camera.Matrix(shader, "camMatrix");

        tex.Bind();
        
        world.update(camera.getPosition());

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

void test() {
    loadSetting();

    PerlinNoise2D test;

    int total;

    float frequency = PROCEDURAL_FREQUENCY;
    float amplitude = PROCEDURAL_AMPLITUDE;

    // for(int y = 0; y < 10; y++) {
    //     for(int x = 0; x < 10; x++) {
    //         total = 0;

    //         for(int o = 0; o < PROCEDURAL_OCTAVES; o++) {
    //             total += test.noise_2d(x*frequency, y*frequency)*amplitude;

    //             amplitude*=PROCEDURAL_PERSISTENCE;
    //             frequency*=PROCEDURAL_MULT_FREQUENCY;
    //         }

    //         std::cout << total << ", ";
    //     }

    //     std::cout << "\n";
    // }

    for(int y = 0; y < 10; y++) {
        for(int x = 0; x < 10; x++) {
            std::cout << test.noise_2d(x*frequency, y*frequency) << ", ";
        }

        std::cout << "\n";
    }
}

int main()
{
    // nullptr = false
    // std::thread threadChunkGeneration(&World::chunkGeneration, &world, &run);

    minecraft();


    // test();

    return 0;
}