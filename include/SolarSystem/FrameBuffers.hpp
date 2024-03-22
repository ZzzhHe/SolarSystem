#pragma once

#include <glad/glad.h>
#include "Shader.hpp"
#include "Model.hpp"
#include "Mesh.hpp"


class FrameBuffer {
    public:
        FrameBuffer() {};
        virtual ~FrameBuffer() {};

        virtual void Bind(int index = 0) = 0;
        virtual void Unbind() = 0;
        virtual void SetBufferToTexture(Shader* shader, int index = 0) = 0;

        virtual void Clear() {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void ActiveTexture(int index = 0) {
            glActiveTexture(GL_TEXTURE0 + index);
        }

        void BindTexture(int index = 0) {
            glBindTexture(GL_TEXTURE_2D, m_colorBuffers[index]);
        }

        void RenderBufferToScreen(Shader * shader);

    protected:
        GLuint m_colorBuffers[2];
};

class HDRFrameBuffer : public FrameBuffer {
    public:
        HDRFrameBuffer(int width, int height);
        ~HDRFrameBuffer() override;

        void Bind(int index = 0) override;
        void Unbind() override;

        void SetupShader(Shader* shader, float exposure);

        void SetBufferToTexture(Shader * shader, int index = 0) override {
            shader->setInt("hdrBuffer", index);
        }

    private:
        GLuint m_FBO;
        GLuint m_rboDepth;
};

class PingpongFrameBuffer : public FrameBuffer{
    public:
        PingpongFrameBuffer(int width, int height);
        ~PingpongFrameBuffer() override;

        void Bind(int index = 0) override;
        void Unbind() override;

        void SetBufferToTexture(Shader * shader, int index = 0) override {
            shader->setInt("pingpongBuffer", index);
        }

    private:
        GLuint m_FBOs[2]; 
};

class DepthMapFrameBuffer: public FrameBuffer {
	public:
		DepthMapFrameBuffer(int width, int height);
		~DepthMapFrameBuffer() override;
		void Bind(int index=0) override;
		void Unbind() override;
		void Clear() override;
	
		void SetBufferToTexture(Shader * shader, int index = 0) override {
			shader->setInt("depthMap", index);
		}

		void BindTexture() {
			glBindTexture(GL_TEXTURE_2D, depthMap);
		}
	
		void CullFrontFace() {
			GLCall(glCullFace(GL_FRONT));
		}
		
		void CullBackFace() {
			GLCall(glCullFace(GL_BACK));
		}
	
	private:
		GLuint depthMapFBO;
		GLuint depthMap;
};

