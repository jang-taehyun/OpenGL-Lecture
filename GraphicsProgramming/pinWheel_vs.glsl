#version 430 core										
														
layout (location = 0) in vec4 color;
layout (location = 1) in float ct;
layout (location = 2) in vec4 v1;
layout (location = 3) in vec4 v2;
layout (location = 4) in vec4 v3;
														
out vec4 vs_color;										
														
void main()												
{
	vec3 cameraPos = vec3(0.2f, 0.3f, 0.8f);
	vec3 targetPos = vec3(0.f, 0.f, 0.5f);
	vec3 upVec = vec3(0.f, 1.f, 0.f);

	vec3 cameraZ = normalize(cameraPos - targetPos);
	vec3 cameraX = cross(upVec, cameraZ);
	vec3 cameraY = cross(cameraZ, cameraX);

	mat4 RenderingCameraMat;
	RenderingCameraMat[0] = vec4(cameraX[0], cameraY[0], cameraZ[0], 0.f);
	RenderingCameraMat[1] = vec4(cameraX[1], cameraY[1], cameraZ[1], 0.f);
	RenderingCameraMat[2] = vec4(cameraX[2], cameraY[2], cameraZ[2], 0.f);
	RenderingCameraMat[3] = vec4(0.f, 0.f, 0.f, 1.f);

	mat4 cameraPosMat;
	cameraPosMat[0] = vec4(1.f, 0.f, 0.f, 0.f);
	cameraPosMat[1] = vec4(0.f, 1.f, 0.f, 0.f);
	cameraPosMat[2] = vec4(0.f, 0.f, 1.f, 0.f);
	cameraPosMat[3] = vec4(-cameraPos[0], -cameraPos[1], -cameraPos[2], 1.f);

	mat4 LookAtMat = RenderingCameraMat * cameraPosMat;

	float left = -0.1f;
	float right = 0.1f;
	float top = 0.07f;
	float bottom = -0.07;
	float near = 0.1f;
	float far = 10.f;

	mat4 perspectiveProjectionMat;
	perspectiveProjectionMat[0] = vec4(2.f * near / (right - left), 0.f, 0.f, 0.f);
	perspectiveProjectionMat[1] = vec4(0.f, 2.f * near / (top - bottom), 0.f, 0.f);
	perspectiveProjectionMat[2] = vec4((right + left) / (right - left), (top + bottom) / (top - bottom), (near + far) / (near - far), -1.f);
	perspectiveProjectionMat[3] = vec4(0.f, 0.f, 2.f * near * far / (near - far), 0.f);

	mat4 rotate;
	rotate[0] = vec4(cos(ct), sin(ct), 0.f, 0.f);
	rotate[1] = vec4(-sin(ct), cos(ct), 0.f, 0.f),
	rotate[2] = vec4(0.f, 0.f, 1.f, 0.f);
	rotate[3] = vec4(0.f, 0.f, 0.f, 1.f);

	mat4 translate;
	translate[0] = vec4(1.f, 0.f, 0.f, 0.f);
	translate[1] = vec4(0.f, 1.f, 0.f, 0.f),
	translate[2] = vec4(0.f, 0.f, 1.f, 0.f);
	translate[3] = vec4(sin(ct) * 0.8f, 0.f, 0.f, 1.f);

	mat4 transMat = perspectiveProjectionMat * LookAtMat * translate;

	const vec4 vertices[3] = vec4[3](v1, rotate * v2, rotate * v3);

	gl_Position = transMat * vertices[gl_VertexID];

	vec4 tmp_color[3] = vec4[3](						
							vec4(color[0], 0.f, 0.f, 1.f),
							vec4(0.f, color[1], 0.f, 1.f),
							vec4(0.f, 0.f, color[2], 1.f));

	vs_color = tmp_color[gl_VertexID%3];
}														