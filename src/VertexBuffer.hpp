#pragma once

#include <glad/glad.h>

class VertexBuffer {
    private:
        GLuint m_RendererID;
    public:
        VertexBuffer(const void* data, GLuint size);
        ~VertexBuffer();

        void Bind() const;
        void UnBind() const;
};