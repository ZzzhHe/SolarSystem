#include "Transform.hpp"

glm::mat4 rotate(glm::mat4 model, glm::vec3 angles) {
	model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return model;
}

glm::mat4 Transform::GetModelMatrix(){
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, m_orbit_center);
	model = rotate(model, m_orbition);
	model = glm::translate(model, glm::vec3(m_radius, 0.0f, 0.0f));
	model = rotate(model, m_rotation);
	model = glm::scale(model, glm::vec3(m_scale));
	this->m_position = glm::vec3(model[3]);
	return model;
}

void Transform::UpdateOrbit(glm::vec3 center, float radius) {
	m_orbit_center = center;
	m_radius = radius;
}

void Transform::UpdateRotation(glm::vec3 rotation) {
	m_rotation = rotation;
}

void Transform::UpdateOrbition(glm::vec3 orbition) {
	m_orbition = orbition;
}

void Transform::UpdateOrbitCenter(glm::vec3 center) {
	m_orbit_center = center;
}

void Transform::UpdateScale(float scale) {
	m_scale = scale;
}

glm::vec3 Transform::GetPosition() const{
	return m_position;
}

std::unique_ptr<Transform> Transform::clone() const {
	return std::make_unique<Transform>(*this);
}
