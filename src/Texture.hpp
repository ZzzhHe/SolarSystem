#pragma once
#include <GLFW/glfw3.h>
#include <string>

enum struct TextureType {
    DIFFUSE,
    SPECULAR,
    EMISSION
};

class Texture {
    private:
        GLuint m_RendererID;
        const std::string m_FilePath;
        const TextureType m_Type;
        unsigned char* m_LocalBuffer;
        int m_Width, m_Height, m_BPP;

    public:
        Texture(const std::string& path, const TextureType type);
        ~Texture();

        void Bind(const unsigned int slot = 0) const;
        void Unbind() const;

        inline int GetWidth() const { return m_Width; }
        inline int GetHeight() const { return m_Height; }

        inline std::string GetFilePath() const { return m_FilePath; }
        inline TextureType GetType() const { return m_Type; }
};