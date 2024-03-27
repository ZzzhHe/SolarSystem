#pragma once

#include "Mesh.hpp"

class FullScreenQuadMesh : public Mesh {
	public:
		FullScreenQuadMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
};

class CircleMesh: public Mesh {
	public:
		CircleMesh(unsigned int pts);
		void Render(Shader* shader) override;
	
		~CircleMesh() override {
			delete this->m_VBO;
			this->m_VBO = nullptr;
			delete this->m_VAO;
			this->m_VAO = nullptr;
			this->m_IBO = nullptr;
		}
	private:
		unsigned int m_pts;
};

class ParticleMesh: public Mesh {
public:
	ParticleMesh() {}
	ParticleMesh(std::vector<float> vertices, std::unique_ptr<Texture> texture);
	void Render(Shader* shader) override;

	~ParticleMesh() override {
		delete this->m_VBO;
		this->m_VBO = nullptr;
		delete this->m_VAO;
		this->m_VAO = nullptr;
		this->m_IBO = nullptr;
	}
private:
	unsigned int m_tris;
	std::unique_ptr<Texture> m_texture;
};
