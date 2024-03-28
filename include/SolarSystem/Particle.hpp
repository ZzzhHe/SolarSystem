#pragma once

#include "OtherMeshes.hpp"
#include "Camera.hpp"

const int PARTICLE_PER_FRAME = 2000;
const float FLARE_PERIOD = 0.5f;

class Particle {
public:
	Particle()
	: m_position(0.0f), m_velocity(0.0f), m_color(1.0f), m_life(0.0f) {}
	
	Particle(glm::vec3 p, glm::vec3 v, glm::vec4 color, float life, float scale)
	: m_position(p), m_velocity(v), m_color(color), m_life(life), m_scale(scale) {}
	
	bool Update(float delta);
	
	void SetPosition(glm::vec3 position);
	
	glm::vec3 GetPosition();
	glm::vec4 GetColor();
	float GetScale();
	
private:
	glm::vec3 m_position, m_velocity;
	glm::vec4 m_color;
	float m_life;
	
	float m_scale = 1.0f;
};

class ParticleManager {
public:
	ParticleManager(unsigned int amount, glm::vec3 pos, float radius, const std::string& path);
	~ParticleManager() {}
	void Update(float delta);
	void Render(Shader* shader);
	unsigned int FirstUnusedParticle();
	Particle RespawnParticle(float theta, float phi);
	
private:
	std::vector<Particle> m_particles;
	unsigned int m_amount;
	std::unique_ptr<ParticleMesh> m_mesh;
	glm::vec3 m_center;
	float m_raduis;
	float m_flarePeriod = FLARE_PERIOD;
};
