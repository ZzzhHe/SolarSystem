#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "GLDebug.hpp"
#include "Model.hpp"


class OutlineRenderer {
    public: 
        void EnableWriting() {
            GLCall(glStencilFunc(GL_ALWAYS, 1, 0xFF)); // set any stencil to 1
            GLCall(glStencilMask(0xFF)); // enable writing to the stencil buffer
        }

        void DisableWriting(){
            GLCall(glStencilFunc(GL_NOTEQUAL, 1, 0xFF)); // only render if the stencil value is not 1
            GLCall(glStencilMask(0x00)); // disable writing to the stencil buffer
            GLCall(glDisable(GL_DEPTH_TEST)); // outline should not be occluded
        }

        void SetupShader(Shader* shader, glm::mat4 model, glm::mat4 view, glm::mat4 projection) {
            shader->Use();
            shader->setMat4("projection", projection);
            shader->setMat4("view", view);
            shader->setMat4("model", model);
            shader->UnUse();
        }

        void Render(Model* model, Shader* shader) {
            model->Render(shader);
        }   
        
        void Clean() {
            GLCall(glStencilMask(0xFF)); 
            GLCall(glStencilFunc(GL_ALWAYS, 0, 0xFF)); 
            GLCall(glEnable(GL_DEPTH_TEST)); 
        }
};
