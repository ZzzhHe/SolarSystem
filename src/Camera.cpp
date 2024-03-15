#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, float yaw, float pitch){
    Front = glm::vec3(0.0f, 0.0f, -1.0f);

    this->Position = position;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::processKeyboard(Movement_Direction direction, float deltaTime) {
    float camSpeed = SPEED * deltaTime;
    if (direction == FORWARD) {
        Position += Front * camSpeed;
    }
    if (direction == BACKWARD) {
        Position -= Front * camSpeed;
    }
    if (direction == RIGHT) {
        Position += glm::normalize(glm::cross(Front, Up)) * camSpeed;
    }
    if (direction == LEFT) {
        Position -= glm::normalize(glm::cross(Front, Up)) * camSpeed;
    }
}

void Camera::processMouse(float xoffset, float yoffset) {
    xoffset *= SENSITIVITY;
    yoffset *= SENSITIVITY;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraVectors() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    Front = glm::normalize(direction);
    Right = glm::normalize(glm::cross(Front, WORLD_UP));
    Up = glm::normalize(glm::cross(Right, Front));  
}