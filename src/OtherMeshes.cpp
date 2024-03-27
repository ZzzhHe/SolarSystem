#include "OtherMeshes.hpp"
#include <iostream>

FullScreenQuadMesh::FullScreenQuadMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
	this->m_VAO = new VertexArray();

	this->m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	
	this->m_IBO = new IndexBuffer(&indices[0], indices.size());

	VertexBufferLayout layout;
	layout.Push<float>(3); // Position
	layout.Push<float>(2); // TexCoords

	this->m_VAO->AddBuffer(*this->m_VBO, layout);
}

CircleMesh::CircleMesh(unsigned int pts) {
	this->m_pts = pts;
	std::vector<float> vertices;
	for( unsigned int idx = 0; idx < pts; ++idx ) {
		float ang = 2.0 * M_PI * float(idx) / pts;
		vertices.push_back(cos(ang));
		vertices.push_back(0.0f);
		vertices.push_back(sin(ang));
	}
	
	this->m_VAO = new VertexArray();
	
	this->m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	
	VertexBufferLayout layout;
	layout.Push<float>(3); // Position
	
	this->m_VAO->AddBuffer(*this->m_VBO, layout);
}

void CircleMesh::Render(Shader * shader){
	shader->Use();
	this->m_VBO->Bind();
	this->m_VAO->Bind();
	GLCall(glDrawArrays(GL_LINE_LOOP, 0, this->m_pts));
	shader->UnUse();
}

ParticleMesh::ParticleMesh(std::vector<float> vertices, std::unique_ptr<Texture> texture)
	: m_texture(std::move(texture)) {
	this->m_VAO = new VertexArray();

	this->m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
	
	VertexBufferLayout layout;
	layout.Push<float>(2); // Position
	layout.Push<float>(2); // TexCoords
	
	this->m_tris = int(vertices.size() / 4); // how many triangles in vertices

	this->m_VAO->AddBuffer(*this->m_VBO, layout);
}

void ParticleMesh::Render(Shader* shader) {
	// shader switch is too time consuming, especially for particles, so we put it outside
	this->m_texture->Bind(0); // can improve here, because only use one texture
	shader->setInt("sprite", 0);
	this->m_VBO->Bind();
	this->m_VAO->Bind();
	GLCall(glDrawArrays(GL_TRIANGLES, 0, this->m_tris));
	this->m_VAO->UnBind();
}
