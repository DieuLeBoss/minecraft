#include "opengl/renderer.hpp"

Renderer::Renderer() {}

int Renderer::init(const char* title_window) {
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

    WIDTH = mode->width;
    HEIGHT = mode->height;

    window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft", primMonitor, NULL);

    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cerr << "Failed to init GLEW !" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Specify the viewport of openGL in the window
    glViewport(0, 0, WIDTH, HEIGHT);

    glEnable(GL_DEPTH_TEST); // permet de ne pas afficher les objets cacher par d'autre 
    glEnable(GL_CULL_FACE); // permet d'activer le culling (affiche les triangles que dans une certaine orientation)

    shader = Shader("res/shaders/defaultV.glsl", "res/shaders/defaultF.glsl");

    texture = Texture("res/textures/terrain.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.textUnit(shader, "tex0", 0);

    return 0;
}

void Renderer::render(Camera& camera, World& world) {
    // Specify the color of the background
    glClearColor(0.07f, 0.13f, 0.917f, 1.0f);
    // Clean the back buffer and assign the new color to it
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    camera.Inputs(window);

    camera.updateMatrix(90.0f, 0.1f, 10000.0f);

    shader.use();
    camera.Matrix(shader, "camMatrix");
    texture.Bind();

    world.draw();

    // Swap the back buffer with the front buffer
    glfwSwapBuffers(window);

    glfwPollEvents();
}

void Renderer::terminate() {
    shader.kill();
    texture.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();
}

GLFWwindow* Renderer::getWindow() {
    return window;
}

int Renderer::getWidth() {
    return WIDTH;
}

int Renderer::getHeight() {
    return HEIGHT;
}