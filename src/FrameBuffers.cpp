#include "FrameBuffers.hpp"
#include "GLDebug.hpp"

#include <iostream>

FullScreenQuadMesh::FullScreenQuadMesh(std::vector<float> vertices, std::vector<unsigned int> indices) {
    this->m_VAO = new VertexArray();

    this->m_VBO = new VertexBuffer(&vertices[0], vertices.size() * sizeof(float));
    
    this->m_IBO = new IndexBuffer(&indices[0], indices.size());

    VertexBufferLayout layout;
    layout.Push<float>(3); // Position
    layout.Push<float>(2); // TexCoords

    this->m_VAO->AddBuffer(*this->m_VBO, layout);
}

void FrameBuffer::Render(Shader* shader) {
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
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width * 2, height * 2));
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

void HDRFrameBuffer::Bind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBO));
}

void HDRFrameBuffer::Unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HDRFrameBuffer::BindTexture() {
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBuffers[0]));
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
    }
}

PingpongFrameBuffer::~PingpongFrameBuffer() {
    GLCall(glDeleteFramebuffers(2, m_FBOs));
    GLCall(glDeleteTextures(2, m_colorBuffers));
}

void PingpongFrameBuffer::Bind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_FBOs[1]));
}

void PingpongFrameBuffer::Unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void PingpongFrameBuffer::BindTexture() {
    GLCall(glActiveTexture(GL_TEXTURE1));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBuffers[0]));
}