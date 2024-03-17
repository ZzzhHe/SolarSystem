#pragma once

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"

class VertexArray {
    private:
        GLuint m_RendererID;
    public:
        VertexArray();
        ~VertexArray();

        void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
        void Bind() const;
        void UnBind() const;
};