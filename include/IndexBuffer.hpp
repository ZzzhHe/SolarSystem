#pragma once

#include <glad/glad.h>

class IndexBuffer {
    private:
        GLuint m_RendererID;
        GLuint m_Count;
    public:
        IndexBuffer(const GLuint* data, GLuint count);
        ~IndexBuffer();

        inline GLuint GetCount() const { return m_Count; }

        void Bind() const;
        void UnBind() const;
};