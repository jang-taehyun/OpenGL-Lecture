// sb7.h ��� ������ ���Խ�Ų��.
#include <sb7.h>
#include <vmath.h>

// sb7::application�� ��ӹ޴´�.
class my_application : public sb7::application
{
public:
	// ���̴� �������Ѵ�.
	GLuint compile_shaders(void)
	{
		GLuint vertex_shader;
		GLuint tcs, tes, gs;
		GLuint fragment_shader;
		GLuint program;

		// ���ؽ� ���̴� �ҽ� �ڵ�
		static const GLchar* vertex_shader_source[] =
		{
			"#version 430 core													\n"
			"																	\n"
			"layout (location = 0) in vec4 offset; 							    \n"
			"layout (location = 1) in vec4 color; 							    \n"
			"out VS_OUT {						 							    \n"
			"	vec4 color;						 							    \n"
			"}	vs_out;							 							    \n"
			"void main(void)													\n"
			"{																	\n"
			"   const vec4 vertices[3] = vec4[3](vec4(0.25, -0.25, 0.5, 1.0),	\n"
			"									 vec4(-0.25, 0.25, 0.5, 1.0),	\n"
			"									 vec4(0.25, 0.25, 0.5, 1.0));	\n"
			"	gl_Position = vertices[gl_VertexID];// + offset;					\n"
			"   const vec4 colors[3] = vec4[3](vec4(1.0, 0.0, 0.0, 1.0),		\n"
			"								   vec4(0.0, 1.0, 0.0, 1.0),		\n"
			"								   vec4(0.0, 0.0, 1.0, 1.0));		\n"
			"	vs_out.color = colors[gl_VertexID];								\n"
			"}																	\n"
		};

		// TCS �ҽ� �ڵ�
		static const GLchar* tcs_source[] =
		{
			"#version 430 core																\n"
			"																				\n"
			"layout (vertices = 3) out;														\n"
			"in VS_OUT {						 											\n"
			"	vec4 color;						 											\n"
			"}	tcs_in[];							 											\n"
			"out vec4 tcs_color[];															\n"
			"																				\n"
			"void main(void)																\n"
			"{																				\n"
			"	if (gl_InvocationID == 0) {													\n"
			"		gl_TessLevelInner[0] = 5.0;												\n"
			"		gl_TessLevelOuter[0] = 5.0;												\n"
			"		gl_TessLevelOuter[1] = 5.0;												\n"
			"		gl_TessLevelOuter[2] = 5.0;												\n"
			"	}																			\n"
			"	gl_out[gl_InvocationID].gl_Position = gl_in[gl_InvocationID].gl_Position;	\n"
			"	tcs_color[gl_InvocationID] = tcs_in[gl_InvocationID].color;					\n"
			"}																				\n"
		};

		// TES �ҽ� �ڵ�
		static const GLchar* tes_source[] =
		{
			"#version 430 core																\n"
			"																				\n"
			"layout (triangles, equal_spacing, cw) in;										\n"
			"in vec4 tcs_color[];															\n"
			"out vec4 tes_color;															\n"
			"																				\n"
			"void main(void)																\n"
			"{																				\n"
			"	gl_Position = (gl_TessCoord.x * gl_in[0].gl_Position + 						\n"
			"				   gl_TessCoord.y * gl_in[1].gl_Position + 						\n"
			"				   gl_TessCoord.z * gl_in[2].gl_Position); 						\n"
			"	tes_color = (gl_TessCoord.x * tcs_color[0] + 						\n"
			"				   gl_TessCoord.y * tcs_color[1] + 						\n"
			"				   gl_TessCoord.z * tcs_color[2]); 						\n"
			"}																				\n"
		};

		// GS �ҽ� �ڵ�
		static const GLchar* gs_source[] =
		{
			"#version 430 core																\n"
			"																				\n"
			"layout (triangles) in;															\n"
			"layout (points, max_vertices = 6) out;											\n"
			"in vec4 tes_color[];															\n"
			"out vec4 gs_color;																\n"
			"																				\n"
			"void main(void)																\n"
			"{																				\n"
			"	int i;																		\n"
			"	for (int i = 0; i < gl_in.length(); i++) {									\n"
			"		gl_Position = gl_in[i].gl_Position;										\n"
			"		gs_color = tes_color[i];												\n"
			"		EmitVertex();															\n"
			"		gl_Position[0] -= 0.3;													\n"
			"		gl_Position[1] -= 0.3;													\n"
			"		gs_color = tes_color[i];												\n"
			"		EmitVertex();															\n"
			"	}																			\n"
			"}																				\n"
		};

		// �����׸�Ʈ ���̴� �ҽ� �ڵ�
		static const GLchar* fragment_shader_source[] =
		{
			"#version 430 core																\n"
			"																				\n"
			"in VS_OUT {																	\n"
			"	vec4 color;																	\n"
			"}	fs_in;																		\n"
			"in vec4 gs_color;																\n"
			"out vec4 color;																\n"
			"																				\n"
			"void main(void)																\n"
			"{																				\n"
			"	color = gs_color;//vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,						\n"
			//"			     cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,						\n"
			//"                sin(gl_FragCoord.x * 0.25) * cos(gl_FragCoord.y * 0.25),		\n"
			//"				 1.0);															\n"
			"}																				\n"
		};

		// ���ؽ� ���̴��� �����ϰ� �������Ѵ�.
		vertex_shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex_shader, 1, vertex_shader_source, NULL);
		glCompileShader(vertex_shader);

		// TCS�� �����ϰ� �������Ѵ�.
		tcs = glCreateShader(GL_TESS_CONTROL_SHADER);
		glShaderSource(tcs, 1, tcs_source, NULL);
		glCompileShader(tcs);

		// TES�� �����ϰ� �������Ѵ�.
		tes = glCreateShader(GL_TESS_EVALUATION_SHADER);
		glShaderSource(tes, 1, tes_source, NULL);
		glCompileShader(tes);

		// GS�� �����ϰ� �������Ѵ�.
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		glShaderSource(gs, 1, gs_source, NULL);
		glCompileShader(gs);

		// �����׸�Ʈ ���̴��� �����ϰ� �������Ѵ�.
		fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment_shader, 1, fragment_shader_source, NULL);
		glCompileShader(fragment_shader);

		// ���α׷��� �����ϰ� ���̴��� Attach��Ű�� ��ũ�Ѵ�.
		program = glCreateProgram();
		glAttachShader(program, vertex_shader);
		glAttachShader(program, tcs);
		glAttachShader(program, tes);
		glAttachShader(program, gs);
		glAttachShader(program, fragment_shader);
		glLinkProgram(program);

		// ���� ���α׷��� ���̴��� �����ϹǷ� ���̴��� �����Ѵ�.
		glDeleteShader(vertex_shader);
		glDeleteShader(tcs);
		glDeleteShader(tes);
		glDeleteShader(gs);
		glDeleteShader(fragment_shader);

		return program;
	}

	// ���ø����̼� �ʱ�ȭ �����Ѵ�.
	virtual void startup()
	{
		rendering_program = compile_shaders();
		glGenVertexArrays(1, &vertex_array_object);
		glBindVertexArray(vertex_array_object);
	}

	// ���ø����̼� ���� �� ȣ��ȴ�.
	virtual void shutdown()
	{
		glDeleteVertexArrays(1, &vertex_array_object);
		glDeleteProgram(rendering_program);
	}

	// ������ virtual �Լ��� �ۼ��ؼ� �������̵��Ѵ�.
	virtual void render(double currentTime)
	{
		const GLfloat red[] = { (float)sin(currentTime) * 0.5f + 0.5f,
			(float)cos(currentTime) * 0.5f + 0.5f,
			0.0f, 1.0f };
		//glClearBufferfv(GL_COLOR, 0, red);

		// ���������� ������ ���α׷� ��ü�� ����ϵ��� �Ѵ�.
		glUseProgram(rendering_program);

		GLfloat attrib[] = { (float)sin(currentTime) * 0.5f, (float)cos(currentTime) * 0.5f, 0.0f, 0.0f };

		// �Է� �Ӽ� 0�� ���� �����Ѵ�. -> �ﰢ�� ������
		//glVertexAttrib4fv(0, attrib);

		GLfloat color[] = { 1.0f, 0.0f, 0.0f, 0.0f };

		// �Է� �Ӽ� 1�� ���� �����Ѵ�. -> �ﰢ�� �÷�
		glVertexAttrib4fv(1, color);

		glPointSize(5.0f);
		glPatchParameteri(GL_PATCH_VERTICES, 3);

		// ������ �׸��� ��带 ���̾����������� ����
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		// �ﰢ���� �ϳ� �׸���.
		glDrawArrays(GL_PATCHES, 0, 3);
	}

private:
	GLuint rendering_program;
	GLuint vertex_array_object;
};

// DECLARE_MAIN�� �ϳ����� �ν��Ͻ�
DECLARE_MAIN(my_application)