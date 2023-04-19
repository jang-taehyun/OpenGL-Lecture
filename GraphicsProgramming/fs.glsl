#version 430 core

in vec3 vs_color;
in vec2 vs_TexCoord;

uniform sampler2D texture1;

out vec4 fs_color;

void main()
{
	fs_color = texture(texture1, vs_TexCoord) * vec4(vs_color, 1.f);
}