#pragma once

#include "Model.hpp"
#include "Transform.hpp"

class SceneObject {
public:
	SceneObject(const char *path, glm::vec3 position, float scale);
	~SceneObject() {}
	
	void Render(Shader* shader);
	
	std::unique_ptr<Transform> transform;
	
private:
	std::unique_ptr<Model> model;
};

class Circle {
public:
	Circle(const unsigned int pts, glm::vec3 position, float scale);
	~Circle() {}
	
	void Render(Shader * shader);
	
	std::unique_ptr<Transform> transform;
	
private:
	std::unique_ptr<Mesh> mesh;
};
