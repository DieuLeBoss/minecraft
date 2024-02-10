#ifndef CAMERA_H
#define CAMERA_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "opengl/shaders.hpp"

class Camera
{
    private:
        glm::vec3 Position;
        glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::mat4 cameraMatrix = glm::mat4(1.0f);
        
        glm::quat OrientationQuat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);


        int width;
        int height;

        float default_speed = 0.05f;
        float run_speed = 0.1f;

        float speed = 0.05f;
        float sensitivity = 100.0f;

    public:

        Camera(int width, int height, glm::vec3 position, GLFWwindow* window);

        void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
        void Matrix(Shader shader, const char* uniform);
        void Inputs(GLFWwindow* window);
        glm::vec3 getPosition();
        glm::vec3* getPointerPosition();

        glm::mat4 getCameraMatrix();
};

#endif