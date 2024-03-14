#pragma once

#include <glad/glad.h>
#include <iostream>

struct PixelData
{
    unsigned int objectID;
    float depth;
};


class PickingTexture {
    public:
        PickingTexture() {}
        ~PickingTexture();
        void Init(unsigned int width, unsigned int height);
        void EnableWriting();
        void DisableWriting();

        PixelData ReadPixel(unsigned int x, unsigned int y);
    
    private:
        GLuint m_fbo = 0;
        GLuint m_pickingTexture = 0;
        GLuint m_depthTexture = 0;

        PixelData m_pixel;
};