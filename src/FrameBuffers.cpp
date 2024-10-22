#include "FrameBuffers.hpp"
#include "GLDebug.hpp"
#include "OtherMeshes.hpp"

#include <iostream>

void FrameBuffer::RenderBufferToScreen(Shader* shader) {
    std::vector<float> quadVertices = {
    // positions        // texCoords
    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     1.0f,  1.0f, 0.0f, 1.0f, 1.0f
    };

    std::vector<unsigned int> quadIndices = {
        0, 1, 2,   // first triangle
        0, 2, 3    // second triangle
    };

    FullScreenQuadMesh quadMesh(quadVertices, quadIndices);
    quadMesh.Render(shader);
}

HDRFrameBuffer::HDRFrameBuffer(int width, int height) : FrameBuffer(){
    GLCall(glGenFramebuffers(1, &m_FBO));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));

    // Create 2 floating point color buffers (1 for normal rendering, other for brightness treshold values)
    GLCall(glGenTextures(2, m_colorBuffers));
    for (int i = 0; i < 2; i ++) {
        GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width * 2, height * 2, 0, GL_RGBA, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_colorBuffers[i], 0));
    }
    
    // Create and attach depth buffer (renderbuffer)
    GLCall(glGenRenderbuffers(1, &m_rboDepth));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width * 8, height * 8));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth));

    // Tell OpenGL which color attachments we'll use (of this framebuffer) for rendering
    unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    GLCall(glDrawBuffers(2, attachments));

    // Check Framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

HDRFrameBuffer::~HDRFrameBuffer() {
    GLCall(glDeleteFramebuffers(1, &m_FBO));
    GLCall(glDeleteTextures(2, m_colorBuffers));
    GLCall(glDeleteRenderbuffers(1, &m_rboDepth));
}

void HDRFrameBuffer::Bind(int index) {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
}

void HDRFrameBuffer::Unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HDRFrameBuffer::SetupShader(Shader* shader, float exposure) {
    shader->setFloat("exposure", exposure);
}



PingpongFrameBuffer::PingpongFrameBuffer(int width, int height) : FrameBuffer() {
    GLCall(glGenFramebuffers(2, m_FBOs));
    GLCall(glGenTextures(2, m_colorBuffers));
    for (int i = 0; i < 2; i ++) {
        GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[i]));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBuffers[i]));
        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width * 2, height * 2, 0, GL_RGBA, GL_FLOAT, nullptr));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
        GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffers[i], 0));
        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer not complete!" << std::endl;
        }
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
    }
}

PingpongFrameBuffer::~PingpongFrameBuffer() {
    GLCall(glDeleteFramebuffers(2, m_FBOs));
    GLCall(glDeleteTextures(2, m_colorBuffers));
}

void PingpongFrameBuffer::Bind(int index) {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[index]));
}

void PingpongFrameBuffer::Unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

DepthMapFrameBuffer::DepthMapFrameBuffer(int width, int height): FrameBuffer() {
	std::cout << "width: " << width << " height: " << height << std::endl;
	GLCall(glGenFramebuffers(1, &depthMapFBO));
	GLCall(glGenTextures(1, &depthMap));
	GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width * 2, height * 2, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
	
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));
	GLCall(glDrawBuffer(GL_NONE));
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not complete!" << std::endl;
	}
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

DepthMapFrameBuffer::~DepthMapFrameBuffer() {
	GLCall(glDeleteFramebuffers(1, &depthMapFBO));
	GLCall(glDeleteTextures(1, &depthMap));
}

void DepthMapFrameBuffer::Bind(int index) {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO));
}

void DepthMapFrameBuffer::Unbind() {
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void DepthMapFrameBuffer::Clear() {
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}
