#version 430 core

layout (location = 0) in vec3 pos;

out vec3 vsColor;

uniform vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(pos, 1.0);
	vsColor = color;
}