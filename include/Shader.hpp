#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader{
    private:
        GLuint m_RendererID;
        std::string m_FilePath;
        std::unordered_map<std::string, int> m_UniformLocationCache;
    public:
        Shader(const std::string& filepath);
        ~Shader();

        void Use() const;
        void UnUse() const;

        void setBool(const std::string &name, bool value);
        void setInt(const std::string &name, int value);
        void setUInt(const std::string &name, unsigned int value);
        void setFloat(const std::string &name, float value);
        void setMat4(const std::string &name, glm::mat4 &value);
        void setVec3(const std::string &name, glm::vec3 &value);
		void setVec2(const std::string &name, glm::vec2 &value);
		void setVec4(const std::string &name, glm::vec4 &value);

private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source);
    GLuint CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

    int GetUniformLocation(const std::string& name);
};
