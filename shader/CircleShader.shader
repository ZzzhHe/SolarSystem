#shader vertex
#version 410 core

layout (location = 0) in vec3 aPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

#shader fragment
#version 410 core

out vec4 FragColor;

void main()
{
	FragColor = 0.12 * vec4(1.0, 1.0, 1.0, 1.0);
}
