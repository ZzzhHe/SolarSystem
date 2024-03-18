#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


glm::mat4 rotate(glm::mat4 model, glm::vec3 angles);

class Transform {
public:
    glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 orbition;
	float scale;

    Transform() 
        :   position(glm::vec3(0.0f, 0.0f, 0.0f)),
            rotation(0.0f),
            scale(1.0f) {}

    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 orbition, float scale)
        : position(position), rotation(rotation), orbition(orbition), scale(scale) {}

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
		model = rotate(model, rotation);
		model = glm::translate(model, position);
		model = rotate(model, orbition);
		model = glm::scale(model, glm::vec3(scale));
        return model;
    }

	void UpdateRotation(glm::vec3 rotation) {
		this->rotation = rotation;
	}

    void UpdateOrbition(glm::vec3 orbition) {
		this->orbition = orbition;
    }
	
	void UpdatePosition(glm::vec3 position) {
		this->position = position;
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
