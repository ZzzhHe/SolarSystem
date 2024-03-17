#pragma once

#include <glm/glm.hpp>

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

enum Movement_Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Camera Value
const float YAW = -120.0f;
const float PITCH = -45.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float FOV = 70.0f; // field of view

class Camera {
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;

        // Euler Angles
        float yaw;
        float pitch;

        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f))
            : Position(position), yaw(YAW), pitch(PITCH) {
                updateCameraVectors();
            }
        Camera(glm::vec3 position, float yaw, float pitch);

        glm::mat4 getViewMatrix();

        void processKeyboard(Movement_Direction direction, float deltaTime) ;

        void processMouse(float xoffset, float yoffset);

    private:
        void updateCameraVectors();
};