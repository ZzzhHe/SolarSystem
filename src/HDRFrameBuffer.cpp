#include "HDRFrameBuffer.hpp"
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

HDRFrameBuffer::HDRFrameBuffer(int width, int height) {
    GLCall(glGenFramebuffers(1, &m_hdrFBO));

    GLCall(glGenTextures(1, &m_colorBuffer));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBuffer));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width * 2, height * 2, 0, GL_RGBA, GL_FLOAT, nullptr));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    
    GLCall(glGenRenderbuffers(1, &m_rboDepth));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, m_rboDepth));
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width * 2, height * 2));

    // Attachments
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorBuffer, 0));
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_rboDepth));

    // Check Framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    }
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

HDRFrameBuffer::~HDRFrameBuffer() {
    GLCall(glDeleteFramebuffers(1, &m_hdrFBO));
    GLCall(glDeleteTextures(1, &m_colorBuffer));
    GLCall(glDeleteRenderbuffers(1, &m_rboDepth));
}

void HDRFrameBuffer::Bind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_hdrFBO));
}

void HDRFrameBuffer::Unbind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void HDRFrameBuffer::Clear() {
    GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}



void HDRFrameBuffer::SetupShader(Shader* shader, float exposure) {
    shader->Use();
    GLCall(glActiveTexture(GL_TEXTURE0));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_colorBuffer));
    shader->setInt("hdrBuffer", 0);
    shader->setFloat("exposure", exposure);
    shader->UnUse();
}

void HDRFrameBuffer::Render(Shader* shader) {
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