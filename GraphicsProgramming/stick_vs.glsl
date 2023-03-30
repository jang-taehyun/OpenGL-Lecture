#version 430 core											
															
layout (location = 0) in vec4 color;
layout (location = 1) in float ct;
layout (location = 2) in vec4 v1;
layout (location = 3) in vec4 v2;
layout (location = 4) in vec4 v3;

out vec4 vs_color;											
															
void main()
{
	mat4 translate;
	translate[0] = vec4(1.f, 0.f, 0.f, 0.f);
	translate[1] = vec4(0.f, 1.f, 0.f, 0.f),
	translate[2] = vec4(0.f, 0.f, 1.f, 0.f);
	translate[3] = vec4(sin(ct) * 0.6f, 0.f, 0.f, 1.f);

	const vec4 vertices[3] = vec4[3](v1, v2, v3);							
	gl_Position = translate * vertices[gl_VertexID];					
	vs_color = color;										
}															