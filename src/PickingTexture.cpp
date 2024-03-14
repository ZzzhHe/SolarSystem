#include "PickingTexture.hpp"
#include "GLDebug.hpp"

void PickingTexture::Init(unsigned int width, unsigned int height) {
    // Create the FBO
    GLCall(glGenFramebuffers(1, &m_fbo));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));

    // Create the texture object for the primitive information buffer
    GLCall(glGenTextures(1, &m_pickingTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_pickingTexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32UI, width, height, 0, GL_RGBA_INTEGER, GL_UNSIGNED_INT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pickingTexture, 0));

    // Create the texture object for the depth buffer
    GLCall(glGenTextures(1, &m_depthTexture));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_depthTexture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0));

    GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (Status != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "FB error, status: 0x%x\n" << Status << std::endl;
        exit(1);
    }

    // Restore the default framebuffer
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

PickingTexture::~PickingTexture() {
    GLCall(glDeleteFramebuffers(1, &m_fbo));
    GLCall(glDeleteTextures(1, &m_pickingTexture));
    GLCall(glDeleteTextures(1, &m_depthTexture));
}

void PickingTexture::EnableWriting() {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_fbo));
}

void PickingTexture::DisableWriting() {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

PixelData PickingTexture::ReadPixel(unsigned int x, unsigned int y) {
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_fbo));
    GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));

    GLCall(glReadPixels(x,y, 1, 1, GL_RGBA_INTEGER, GL_UNSIGNED_INT, &m_pixel));
    
    GLCall(glReadBuffer(GL_NONE));
    GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
    
    return m_pixel;
}