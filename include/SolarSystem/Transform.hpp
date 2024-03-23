#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
	Transform(glm::vec3 position, float scale)
	: m_position(position), m_scale(scale) {}
	
	glm::mat4 GetModelMatrix() const;
	
	void UpdateOrbit(glm::vec3 center, float radius);
	
	void UpdateRotation(glm::vec3 rotation);
	
	void UpdateOrbition(glm::vec3 orbition);
	
	void UpdateOrbitCenter(glm::vec3 center);
	
	glm::vec3 GetPosition() const;
	
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
