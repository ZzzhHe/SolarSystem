#pragma once
#include "Shader.hpp"
#include <iostream>

class Light {

    public:
        Light(const glm::vec3& color, float intensity)
            : color(color), intensity(intensity) {}

        virtual void setColor(const glm::vec3& color) { this->color = color; };
        virtual void setIntensity(float intensity) { this->intensity = intensity; };

        virtual void SetupShader(Shader* shader) = 0;


    protected: 
        glm::vec3 color;
        float intensity;

};

class PointLight : public Light{
    public:
        PointLight(const glm::vec3& position, const glm::vec3& color)
            : Light(color, 1.0f), position(position), constant(1.0f), linear(0.0014f), quadratic(0.000007f), 
                ambient(color * 0.1f), diffuse(color * 0.7f), specular(color * 1.0f) {}

        void setPosition(const glm::vec3& position) { this->position = position; }
        void setIntensity(float intensity) override { this->intensity = intensity; }


        void SetupShader(Shader* shader) override {
            shader->Use();
            shader->setVec3("pointLight.position", position);
            shader->setVec3("pointLight.ambient", ambient);
            shader->setVec3("pointLight.diffuse", diffuse);
            shader->setVec3("pointLight.specular", specular);
            shader->setFloat("pointLight.constant", constant);
            shader->setFloat("pointLight.linear", linear);
            shader->setFloat("pointLight.quadratic", quadratic);
            shader->setFloat("pointLight.intensity", intensity);
            shader->UnUse();
        }

    private:
        glm::vec3 position;

        float constant;
        float linear;
        float quadratic;
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
};