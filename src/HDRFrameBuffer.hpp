#pragma once

#include <glad/glad.h>
#include "Shader.hpp"
#include "Model.hpp"
#include "Mesh.hpp"

class HDRFrameBuffer {
    public:
        HDRFrameBuffer(int width, int height);
        ~HDRFrameBuffer();

        void Bind();
        void Unbind();
        void Clear();

        void SetupShader(Shader* shader, float exposure);

        void Render(Shader* shader);

    private:
        GLuint m_hdrFBO;
        GLuint m_colorBuffer;
        GLuint m_rboDepth;
};

class FullScreenQuadMesh : public Mesh {
    public:
        FullScreenQuadMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
};  