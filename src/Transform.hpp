#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    Transform() 
        :   position(glm::vec3(0.0f, 0.0f, 0.0f)),
            rotation(glm::vec3(0.0f, 0.0f, 0.0f)),
            scale(glm::vec3(1.0f, 1.0f, 1.0f)) {}

    Transform(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) 
        : position(position), rotation(rotation), scale(scale) {}

    glm::mat4 GetModelMatrix() const {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));
        model = glm::scale(model, scale);
        return model;
    }

    void SetRotation(glm::vec3 newRotation) {
        rotation = newRotation;
    }

    void SetPosition(glm::vec3 newPosition) {
        position = newPosition;
    }

    void SetScale(glm::vec3 newScale) {
        scale = newScale;
    }
};