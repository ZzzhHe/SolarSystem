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
    // Normal = aNormal;
}

#shader fragment
# version 410 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    sampler2D diffuse; 
    sampler2D specular;
    sampler2D emission;
};

// Directional Light
struct DirectLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float intensity;
};
uniform DirectLight directLight;

// Light
uniform vec3 viewPos;
uniform Material material;

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir);

void main() {
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 direct_light = CalcDirectLight(directLight, norm, viewDir);
    FragColor = vec4(direct_light, 1.0);
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if (brightness > 0.9) {
        BrightColor = vec4(FragColor.rgb, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}

vec3 CalcDirectLight(DirectLight light, vec3 normal, vec3 viewDir) {
    vec3 lightDirection = normalize(-light.direction);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
    // diffuse 
    float diff = max(dot(normal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    
    // specular
    vec3 halfwayDir = normalize(lightDirection + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0f);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

    // twilight zone
    float emit = 1.0f;
    if (diff > 0.4f) {
        emit = 0.0f;
    } else if (diff > 0.375f) {
        emit = 0.05f;
    }  else if (diff > 0.35f) {
        emit = 0.1f;
    } else if (diff > 0.325f) {
        emit = 0.15f;
    } else if (diff > 0.3f) {
        emit = 0.2f;
    } else if (diff > 0.2f) {
        emit = 0.4f;
    } else if (diff > 0.15f) {
        emit = 0.55f;
    } else if (diff > 0.125f) {
        emit = 0.7f;
    } else if (diff > 0.1f) {
        emit = 0.8f;
    } else if (diff > 0.075f) {
        emit = 0.85f;
    } else if (diff > 0.05f) {
        emit = 0.9f;
    } else if (diff > 0.025f) {
        emit = 0.95f;
    }
    
    vec3 emission = texture(material.emission, TexCoords).rgb * emit;

    return (ambient + diffuse + specular + emission) * light.intensity;
} 
