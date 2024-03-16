#pragma once

#include <glad/glad.h>
#include "Shader.hpp"
#include "Model.hpp"
#include "Mesh.hpp"


class FrameBuffer {
    public:
        FrameBuffer() {};
        virtual ~FrameBuffer() {};

        virtual void Bind() = 0;
        virtual void Unbind() = 0;
        void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        virtual void BindTexture() = 0;

        void Render(Shader * shader);

    protected:
        GLuint m_colorBuffers[2];
};

class HDRFrameBuffer : public FrameBuffer {
    public:
        HDRFrameBuffer(int width, int height);
        ~HDRFrameBuffer() override;

        void Bind() override;
        void Unbind() override;

        void BindTexture() override;

        void SetupShader(Shader* shader, float exposure);

    private:
        GLuint m_FBO;
        GLuint m_rboDepth;
};

class PingpongFrameBuffer : public FrameBuffer{
    public:
        PingpongFrameBuffer(int width, int height);
        ~PingpongFrameBuffer() override;

        void Bind() override;
        void Unbind() override;

        void BindTexture() override;
    private:
        GLuint m_FBOs[2]; 
};

class FullScreenQuadMesh : public Mesh {
    public:
        FullScreenQuadMesh(std::vector<float> vertices, std::vector<unsigned int> indices);
};  