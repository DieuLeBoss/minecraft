#include "camera.hpp"

Camera::Camera() {}

Camera::Camera(int width, int height, glm::vec3 position, GLFWwindow* window)
{
    this->width = width;
    this->height = height;
    Position = position;

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    glfwSetCursorPos(window, (double)width/2, (double)height/2);
}

void Camera::updateMatrix(float FOVdeg, float nearPlane, float farPlane)
{
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    view = glm::lookAt(Position, Position + Orientation, Up);
    //projection = glm::perspective(glm::radians(FOVdeg), (float)(width / height), nearPlane, farPlane);

    projection = glm::perspective(glm::radians(FOVdeg), static_cast<float>(width) / static_cast<float>(height), nearPlane, farPlane);

    cameraMatrix = projection * view;
}

void Camera::Matrix(Shader shader, const char* uniform)
{
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(cameraMatrix));
}

void Camera::Inputs(GLFWwindow* window)
{   
    right = glm::normalize(glm::cross(Orientation, Up));
    front = glm::normalize(glm::cross(Up, right));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += speed * front;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position += speed * -front;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += speed * right;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position += speed * -right;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += speed * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
    {
        Position += speed * -Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        speed = run_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)
    {
        speed = default_speed;
    }

    double posX, posY;
    
    glfwGetCursorPos(window, &posX, &posY);

    float rotX = sensitivity * (float)(posY - (height / 2)) / height;
    float rotY = sensitivity * (float)(posX - (width / 2)) / width;

    glm::vec3 newOrientation = glm::rotate(Orientation, glm::radians(-rotX), glm::normalize(glm::cross(Orientation, Up)));

    float tmp = glm::angle(newOrientation, Up);

    if (tmp < glm::radians(170.0f) && tmp > glm::radians(10.0f)) // ça reste temporaire et ne résoud pas le probleme voir strat.txt pour le reste
        Orientation = newOrientation;

    Orientation = glm::rotate(Orientation, glm::radians(-rotY), Up);

    glfwSetCursorPos(window, (double)width/2, (double)height/2);
}

glm::vec3 Camera::getPosition()
{
    return this->Position;
}

glm::vec3* Camera::getPointerPosition() {
    return &Position;
}

glm::mat4 Camera::getCameraMatrix()
{
    return cameraMatrix;
}