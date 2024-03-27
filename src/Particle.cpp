#include "Particle.hpp"

float randomFloat(float min, float max) {
	return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(max-min)));
}

void Particle::SetPosition(glm::vec3 position) {
	m_position = position;
}

glm::vec3 Particle::GetPosition() {
	return m_position;
}

glm::vec4 Particle::GetColor() {
	return m_color;
}

float Particle::GetScale() {
	return m_scale;
}

bool Particle::Update(float delta) { // per frame
	// attenuation
	m_velocity *= 0.5;
	m_scale *= 0.99;
	m_position += m_velocity * delta;
	m_color.a -= delta * 0.1f;
	m_life -= delta;
	return m_life >= 0 && m_color.a > 0.0f;
}


ParticleManager::ParticleManager(unsigned int amount, glm::vec3 pos, float r, const std::string& path)
	: m_amount(amount), m_center(pos), m_raduis(r) {
	std::vector<float> vertices = {
		// pos		// texcoord
		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 0.0f,

		0.0f, 1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 1.0f, 0.0f
	};
//	std::unique_ptr<Texture> texture = std::make_unique<Texture>(path, TextureType::DIFFUSE);
	this->m_mesh = std::make_unique<ParticleMesh>(vertices, std::make_unique<Texture>(path, TextureType::DIFFUSE));
}

void ParticleManager::Render(Shader* shader) {
	GLCall(glDepthMask(GL_FALSE));
	glm::vec3 pos;
	glm::vec4 color;
	float scale;
	shader->Use();
	for (Particle particle: this->m_particles) {
		pos = particle.GetPosition();
		color = particle.GetColor();
		scale = particle.GetScale();
		shader->setVec3("offset", pos);
		shader->setVec4("color", color);
		shader->setFloat("scale", scale);
		m_mesh->Render(shader);
	}
	shader->UnUse();
	GLCall(glDepthMask(GL_TRUE));
}

void ParticleManager::Update(float delta) {
	for (int i = 0; i < m_amount / 100 && m_particles.size() < m_amount; ++ i) {
		m_particles.push_back(RespawnParticle());
	}
	
	for (int i = 0; i < m_particles.size(); ++ i) {
		bool stillAlive = m_particles[i].Update(delta);
		if (!stillAlive) {
			std::swap(m_particles[i], m_particles.back());
			m_particles.pop_back();
			--i;
		}
	}
}

Particle ParticleManager::RespawnParticle() {
	// Generate a random direction
	float theta = randomFloat(0.0f, 2.0f * M_PI); // Random angle around Y axis [0, 2PI]
	float phi = randomFloat(0.0f, M_PI); // Random angle from Y axis [0, PI]
	// Convert spherical coordinates to Cartesian coordinates for the direction
	glm::vec3 direction = glm::vec3(sin(phi) * cos(theta), cos(phi), sin(phi) * sin(theta));
	glm::vec3 p = m_center + direction * m_raduis;
	
	float magnitude = randomFloat(0.01f, 0.05f);
	glm::vec3 v = direction * magnitude;
	
	float rColor = randomFloat(0.5f, 1.0f);
	glm::vec4 color = glm::vec4(rColor, rColor, rColor, 1.0f);
	
	float life = randomFloat(20.0f, 80.0f);

	return Particle(p, v, color, life);
}
