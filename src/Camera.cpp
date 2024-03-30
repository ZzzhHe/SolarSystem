#include "Camera.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::processKeyboard(Movement_Direction direction, float deltaTime) {
    float camSpeed = 0.1f * SPEED * deltaTime;
    if (direction == FORWARD) {
		if (focusDistance > 20.0f) {
			focusDistance -= camSpeed * 30.0f;
		} else if (focusDistance > 4.0f and focusDistance < 5.0f) {
			focusDistance -= camSpeed * 0.5f;
		} else if (focusDistance < 0.7f) {
			focusDistance = 0.7f;
		} else {
			focusDistance -= focusDistance * camSpeed;
		}
    }
    if (direction == BACKWARD) {
		focusDistance += camSpeed * 5.0f;
    }
    if (direction == RIGHT) {
        Position += glm::normalize(glm::cross(Front, Up)) * camSpeed * 1.25f;
    }
    if (direction == LEFT) {
        Position -= glm::normalize(glm::cross(Front, Up)) * camSpeed * 1.25f;
    }
	targetFocusDistance = focusDistance;
}

void Camera::focusOn(glm::vec3 planet_position) {
	glm::vec3 direction = glm::normalize(planet_position - Position);
	
	TargetPosition = planet_position - direction * targetFocusDistance;
	
	Front = direction;
	
	Right = glm::normalize(glm::cross(Front, WORLD_UP));
	Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::update(float deltaTime) {
	float lerpRate = 5.0f * deltaTime; // Control the speed of interpolation

	// Interpolate the position and focus distance
	Position = glm::mix(Position, TargetPosition, lerpRate);
	focusDistance = glm::mix(focusDistance, targetFocusDistance, lerpRate);

	// Update Front vector based on the new Position
	glm::vec3 direction = glm::normalize(TargetPosition - Position);
	Front = direction;
	Right = glm::normalize(glm::cross(Front, WORLD_UP));
	Up = glm::normalize(glm::cross(Right, Front));
}
