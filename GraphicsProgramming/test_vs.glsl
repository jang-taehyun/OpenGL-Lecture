#version 430 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 color;

uniform mat4 transMat;

out vec3 vs_color;

void main()
{
	gl_Position = transMat * vec4(pos.x, pos.y, pos.z, 1.f);

	vs_color = color;
}