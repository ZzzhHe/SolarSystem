#shader vertex
#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{   
    TexCoords = aTexCoord;
    gl_Position = vec4(aPos, 1.0f);
}

#shader fragment
#version 410 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D hdrBuffer;
uniform sampler2D pingpongBuffer;
uniform float exposure;

void main()
{
    const float gamma = 2.2;
    vec3 hdrColor = texture(hdrBuffer, TexCoords).rgb;
    
    // bloom
    vec3 bloomColor = texture(pingpongBuffer, TexCoords).rgb;
    hdrColor += bloomColor;
    
	// tone mapping
    vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);
    
    vec3 result = pow(mapped, vec3(1.0 / gamma)); // gamma correct
    FragColor = vec4(result, 1.0);
}
