#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 rotate(glm::mat4 model, glm::vec3 angles);

class Transform {
public:
    glm::vec3 circle_center;
	float radius;
	glm::vec3 rotation;
	glm::vec3 orbition;
	float scale;

    Transform() 
        :   circle_center(glm::vec3(0.0f, 0.0f, 0.0f)),
			radius(0.0f),
            rotation(0.0f),
			orbition(0.0f),
            scale(1.0f) {}

    Transform(glm::vec3 circle_center, float radius = 0.0f, float scale = 1.0f, 
			  glm::vec3 rotation = glm::vec3(0.0f), glm::vec3 orbition = glm::vec3(0.0f))
        : circle_center(circle_center), radius(radius), rotation(rotation), orbition(orbition), scale(scale) {}

    glm::mat4 GetModelMatrix() const {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, circle_center);
		model = rotate(model, orbition);
		model = glm::translate(model, glm::vec3(radius, 0.0f, 0.0f));
		model = rotate(model, rotation);
		model = glm::scale(model, glm::vec3(scale));
        return model;
    }
	
	void UpdateRotation(glm::vec3 rotation) {
		this->rotation = rotation;
	}

    void UpdateOrbition(glm::vec3 orbition) {
		this->orbition = orbition;
    }
	
	void UpdateCenter(glm::vec3 circle_center) {
		this->circle_center = circle_center;
	}
	
	void UpdateRadius(float radius) {
		this->radius = radius;
	}

    void SetScale(float scale) {
		this->scale = scale;
    }
};

glm::mat4 rotate(glm::mat4 model, glm::vec3 angles) {
	model = glm::rotate(model, glm::radians(angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
	return model;
}
