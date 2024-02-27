#include <glad/glad.h>

#include "Texture.hpp"
#include "GLDebug.hpp"
#include <iostream>

#include "stb_image/stb_image.h"


Texture::Texture(const std::string& path, const TextureType type)
    : m_RendererID(0), m_FilePath(path), m_Type(type), m_Width(0), m_Height(0), m_BPP(0) {
    // flip texture(top left 0) upside down to fit OpenGL's bottom left 0
    stbi_set_flip_vertically_on_load(true);

    m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

    GLCall(glGenTextures(1, &m_RendererID));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

    // set wrapping
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    // set filtering
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    GLenum format;
    if (m_BPP == 1) {
        format = GL_RED;
    } else if (m_BPP == 3) {
        format = GL_RGB;
    } else if (m_BPP == 4) {
        format = GL_RGBA;
    }

    GLCall(glTexImage2D(GL_TEXTURE_2D,
                    0, 
                    format, 
                    m_Width, 
                    m_Height, 
                    0, 
                    format, 
                    GL_UNSIGNED_BYTE, 
                    m_LocalBuffer));
    GLCall(glGenerateMipmap(GL_TEXTURE_2D));

    glBindTexture(GL_TEXTURE_2D, 0);
    if (m_LocalBuffer) {
        stbi_image_free(m_LocalBuffer);
    }
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(const unsigned int slot) const {
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}