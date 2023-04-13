#version 430 core

in vec3 vs_color;
out vec4 fs_color;

void main()
{
	fs_color = vec4(vs_color.r, vs_color.g, vs_color.b, 1.f);
}