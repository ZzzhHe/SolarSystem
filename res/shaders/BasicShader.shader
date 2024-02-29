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
    sampler2D diffuse; 
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

// PointLight
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform PointLight pointLight;

// Light
uniform vec3 viewPos;
uniform Material material;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 point_light = CalcPointLight(pointLight, norm, FragPos, viewDir);
    FragColor = vec4(point_light, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir) {
    vec3 lightDirection = normalize(light.position - fragPos);

    // ambient
    vec3 ambient  = light.ambient  * vec3(texture(material.diffuse, TexCoords));

    // diffuse
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(material.diffuse, TexCoords));
    
    // specular
    vec3 reflectDir = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    

    float emit = 1 - diff;
    vec3 emission = texture(material.emission, TexCoords).rgb * emit;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    emission *= attenuation;

    return (ambient + diffuse + specular + emission);
} 