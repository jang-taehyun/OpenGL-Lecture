#version 430 core											
															
layout(location = 0) in vec3 pos;							
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;							
layout(location = 3) in vec3 normal;
															
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
															
out vec3 vsNormal;
out vec3 vsPos;
out vec2 vsTexCoord;
															
void main(void)
{
	gl_Position = projection * view * model * vec4(pos.x, pos.y, pos.z, 1.0);

	vsPos = vec3(model * vec4(pos, 1.f));
	vsNormal = mat3(transpose(inverse(model))) * normal;
	vsTexCoord = texCoord;
}															