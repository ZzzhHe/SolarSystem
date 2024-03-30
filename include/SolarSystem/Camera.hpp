#pragma once

#include <glm/glm.hpp>

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);

enum Movement_Direction {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

enum Focus_Objects {
	EARTH,
	MOON,
	SUN
};

// Camera Value
const float SPEED = 2.5f;
const float FOV = 60.0f; // field of view
const float NEAR = 1.0f;

class Camera {
    public:
        glm::vec3 Position;
        glm::vec3 Front;
        glm::vec3 Up;
        glm::vec3 Right;
		glm::vec3 TargetPosition;
	
		float focusDistance = NEAR;
		float targetFocusDistance = NEAR;
	
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f))
            : Position(position) {}

        glm::mat4 getViewMatrix();

        void processKeyboard(Movement_Direction direction, float deltaTime) ;
	
		void focusOn(glm::vec3 planet_position);
	
		void update(float deltaTime);
};
