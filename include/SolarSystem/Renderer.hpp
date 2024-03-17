#pragma once

#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Shader.hpp"

class Renderer {
    public:
        void Draw (const VertexArray& va, const IndexBuffer& ib) const;
        void Clear () const;
};