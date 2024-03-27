#shader vertex
#version 410 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aTexCoord;

out vec2 TexCoords;
out vec4 ParticleColor;

uniform mat4 projection;
uniform mat4 view;
uniform vec3 cameraUp;
uniform vec3 cameraRight;

uniform vec3 offset;
uniform vec4 color;
uniform float scale;

void main() {
	TexCoords = aTexCoord;
	ParticleColor = color;
	vec3 pos = offset + (cameraRight * aPos.x + cameraUp * aPos.y) * scale;
	gl_Position = projection * view * vec4(pos, 1.0f);
}

#shader fragment
#version 410 core

layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;
in vec4 ParticleColor;

uniform sampler2D sprite;

void main() {
	FragColor = 1.5f * texture(sprite, TexCoords) * ParticleColor;
	if (FragColor.a < 0.05) {
		discard;
	}
}
