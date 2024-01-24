#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"


GLfloat vertices[] = {
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,   0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.5f, 1.0f,   0.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.5f,   0.0f, 0.0f, 1.0f,

    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,   -1.0f, 0.0f,  0.0f, 
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.5f, 1.0f,   -1.0f, 0.0f,  0.0f, 
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   -1.0f, 0.0f,  0.0f, 
    1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.5f,   -1.0f, 0.0f,  0.0f, 

    1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,   0.0f, 0.0f, -1.0f, 
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 1.0f,   0.0f, 0.0f, -1.0f, 
    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   0.0f, 0.0f, -1.0f, 
    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.5f,   0.0f, 0.0f, -1.0f, 

    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,   1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 1.0f,   1.0f, 0.0f, 0.0f, 
    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 1.0f,   1.0f, 0.0f, 0.0f, 
    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   1.0f, 0.5f,   1.0f, 0.0f, 0.0f, 

    0.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   0.0f, -1.0f, 0.0f, 
    0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.5f,   0.0f, -1.0f, 0.0f, 
    1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,   0.0f, -1.0f, 0.0f, 
    1.0f, 1.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.0f,   0.0f, -1.0f, 0.0f, 

    0.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 1.0f,   0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,   0.0f, 0.0f, 0.0f,   0.5f, 1.0f,   0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.5f, 0.5f,   0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.5f,   0.0f, 1.0f, 0.0f,
};

GLuint indices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    8, 9, 10,
    10, 11, 8,

    12, 13, 14,
    14, 15, 12,

    16, 17, 18,
    18, 19, 16,

    20, 21, 22,
    22, 23, 20
};

GLfloat lightVertices[] = {
    3.0f, 2.0f, 1.0f,  //0 
    4.0f, 2.0f, 1.0f,  //1
    4.0f, 2.0f, 2.0f,  //2
    3.0f, 2.0f, 2.0f,  //3

    3.0f, 3.0f, 1.0f,  //4
    4.0f, 3.0f, 1.0f,  //5
    4.0f, 3.0f, 2.0f,  //6
    3.0f, 3.0f, 2.0f,  //7
};

GLuint lightIndices[] = {
    0, 1, 2,
    2, 3, 0,

    4, 5, 6,
    6, 7, 4,

    0, 4, 5,
    5, 1, 0,

    1, 5, 6,
    6, 2, 1,

    2, 6, 7,
    7, 3, 2,

    3, 7, 4,
    4, 0, 3

    // a finir "LIGHTING"
};


int main()
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

    #pragma region "Importation of shaders and textures, and creation and bind of buffers"

    Shader shader("res/shaders/defaultV.glsl", "res/shaders/defaultF.glsl");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkVBO(VBO1, 0, 3, GL_FLOAT, 11 * sizeof(float), (void*)0);
    VAO1.LinkVBO(VBO1, 1, 3, GL_FLOAT, 11 * sizeof(float), (void*)(3 * sizeof(float)));
    VAO1.LinkVBO(VBO1, 2, 2, GL_FLOAT, 11 * sizeof(float), (void*)(6 * sizeof(float)));
    VAO1.LinkVBO(VBO1, 3, 3, GL_FLOAT, 11 * sizeof(float), (void*)(8 * sizeof(float)));
    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();




    Shader lightShader("res/shaders/lightV.glsl", "res/shaders/lightF.glsl");

    VAO lightVAO;
    lightVAO.Bind();

    VBO VBO2(lightVertices, sizeof(lightVertices));
    EBO EBO2(lightIndices, sizeof(lightIndices));

    lightVAO.LinkVBO(VBO2, 0, 3, GL_FLOAT, 3 * sizeof(float), (void*)0);
    lightVAO.Unbind();
    VBO2.Unbind();
    EBO2.Unbind();




    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::mat4(1.0f);
	lightModel = glm::translate(lightModel, lightPos);

	glm::vec3 pyramidPos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramidModel = glm::mat4(1.0f);
	pyramidModel = glm::translate(pyramidModel, pyramidPos);


	lightShader.use();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
	glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	shader.use();
	glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(pyramidModel));
	glUniform4f(glGetUniformLocation(shader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shader.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);



    Texture tex("res/textures/texture.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    tex.textUnit(shader, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    #pragma endregion

    Camera camera(WIDTH, HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f), window);

    while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
    {
        // Specify the color of the background
        glClearColor(0.07f, 0.13f, 0.917f, 1.0f);
        // Clean the back buffer and assign the new color to it
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.Inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        shader.use();

        glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
        camera.Matrix(shader, "camMatrix");

        tex.Bind();
        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);


        lightShader.use();

        camera.Matrix(lightShader, "camMatrix");
        
        lightVAO.Bind();
        
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(int), GL_UNSIGNED_INT, 0);

        // Swap the back buffer with the front buffer
        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shader.kill();

    lightVAO.Delete();
    VBO2.Delete();
    EBO2.Delete();
    lightShader.kill();

    tex.Delete();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
