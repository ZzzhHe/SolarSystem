#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 trans;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    gl_Position = projection * view * model * trans * vec4(aPos, 1.0f); 
    TexCoords = aTexCoord;
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    // Normal = aNormal;
}

#shader fragment
# version 410 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D ambient;
    sampler2D diffuse; 
    sampler2D specular;
    // sampler2D height;
    float shininess;
};

// Directional Light
struct DirectLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectLight directLight;

// Light
uniform vec3 viewPos;
uniform Material material;

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    // directional light
    vec3 direct_light = CalcDirectLight(directLight, norm, viewDir);
    FragColor = vec4(direct_light, 1.0);
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDirection = normalize(-light.direction);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.ambient, TexCoords));
    
    // diffuse 
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // specular
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);    
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    
    return (ambient + diffuse + specular);
}