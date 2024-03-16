#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f); 
    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
}

#shader fragment
#version 410 core
layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Material {
    sampler2D emission;
};

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform Material material;

void main() {
    vec3 emission = 4.5f * texture(material.emission, TexCoords).rgb;
    FragColor = vec4(emission, 1.0f);
    BrightColor = vec4(emission, 1.0f);
}
