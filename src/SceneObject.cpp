#include "SceneObject.hpp"
#include "OtherMeshes.hpp"

SceneObject::SceneObject(const char *path,
						 glm::vec3 position,
						 float scale) {
	model = std::make_shared<Model>(path);
	transform = std::make_shared<Transform>(position, scale);
}

void SceneObject::Render(Shader * shader) {
	shader->Use();
	glm::mat4 M = transform->GetModelMatrix();
	shader->setMat4("model", M);
	model->Render(shader);
	shader->UnUse();
}

Circle::Circle(const unsigned int pts,
			   glm::vec3 position,
			   float scale) {
	
	mesh = std::make_shared<CircleMesh>(pts);
	
	transform = std::make_shared<Transform>(position, scale);
}

void Circle::Render(Shader * shader) {
	shader->Use();
	glm::mat4 M = transform->GetModelMatrix();
	shader->setMat4("model", M);
	mesh->Render(shader);
	shader->UnUse();
}
