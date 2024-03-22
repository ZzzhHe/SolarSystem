#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 rotate(glm::mat4 model, glm::vec3 angles);

class Transform {
public:
	Transform(glm::vec3 position, float scale)
	: m_position(position), m_scale(scale) {}
	
	glm::mat4 GetModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, m_orbit_center);
		model = rotate(model, m_orbition);
		model = glm::translate(model, glm::vec3(m_radius, 0.0f, 0.0f));
		model = rotate(model, m_rotation);
		model = glm::scale(model, glm::vec3(m_scale));
		return model;
	}
	
	void UpdateOrbit(glm::vec3 center, float radius) {
		m_orbit_center = center;
		m_radius = radius;
	}
	
	void UpdateRotation(glm::vec3 rotation) {
		m_rotation = rotation;
	}
	
	void UpdateOrbition(glm::vec3 orbition) {
		m_orbition = orbition;
	}
	
	void UpdateOrbitCenter(glm::vec3 center) {
		m_orbit_center = center;
	}
	
	glm::vec3 GetPosition() {
		return m_position;
	}
	
	
private:
	glm::vec3 m_position;
	float m_scale;
	// rotate, default still
	glm::vec3 m_rotation = glm::vec3(0.0f);
	// orbit, default at the center
	glm::vec3 m_orbit_center = glm::vec3(0.0f);
	float m_radius = 0.0f;
	glm::vec3 m_orbition = glm::vec3(0.0f);
};


glm::mat4 rotate(glm::mat4 model, glm::vec3 angles) {
	model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return model;
}
